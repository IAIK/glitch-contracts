import os
from time import sleep
from datetime import datetime, timedelta
import sys
import signal
import logging
import multiprocessing
from tqdm import tqdm
import queue
from typing import Optional, Callable

NUM_CPUS = max(os.cpu_count() - 2, 1)
MP_CONTEXT = multiprocessing.get_context('fork')


class SyncMarker:
    def __init__(self, id):
        self._id = id
        
    def __eq__(self, other):
        return self._id == other._id
    
    def __repr__(self):
        return f'SyncMarker({repr(self._id)})'
    
    def tick(self):
        self._id += 1
        
        
initial_sync_marker = SyncMarker(0)
        
class Multipipe:
    def __init__(self):
        self.send_connections = []
        self.recv_connections = []
        self.syncmarker = initial_sync_marker
        self.used_counter = MP_CONTEXT.Value('i', 0)
            
    def set_n_pipes(self, n_connections):
        assert len(self.send_connections) == 0
        for _ in range(n_connections):
            self.new_pipe()
        
    def new_pipe(self):
        read_conn, write_conn = MP_CONTEXT.Pipe(duplex=False)
        self.send_connections.append(write_conn)
        self.recv_connections.append(read_conn)
    
    def send(self, obj):
        self.syncmarker.tick()
        for pipe in self.send_connections:
            pipe.send(obj)
            pipe.send(self.syncmarker)
            
    def close(self):
        for pipe in self.send_connections:
            pipe.close()
            
    def get_recv_end(self):
        with self.used_counter.get_lock():
            ret = self.recv_connections[self.used_counter.value]
            self.used_counter.value += 1
            return ret


multifun: Optional[Callable] = None
multiqueue_recv: "Connection" = None
local_queue = []
last_syncmarker: SyncMarker = initial_sync_marker

def new_messages():
    global local_queue
    ret = local_queue
    local_queue = []
    return ret

def recv_until(syncmark):
    global local_queue, last_syncmarker
    if syncmark == last_syncmarker:
        #print('xxxxx nothing to sync', multiprocessing.current_process())
        return
    while True:
        #print('xxxxxx waiting for multipacket', multiprocessing.current_process())
        obj = multiqueue_recv.recv() # TODO remove timeout
        #print('recved multipacket', obj, multiprocessing.current_process())
        if isinstance(obj, SyncMarker):
            #print('checking if done', obj == syncmark, obj, syncmark, multiprocessing.current_process())
            if obj == syncmark:
                last_syncmarker = syncmark
                break
        else:
            local_queue.append(obj)

def set_multifun(fun, multipipe=None):
    global multifun, multiqueue_recv
    if multipipe:
        multiqueue_recv = multipipe.get_recv_end()
    multifun = fun
    
def run_multifun_expand(args):
    return multifun(*args)
    
def run_multifun(*args, **kwargs):
    #print('run_multifun', args, kwargs)
    if 'syncmark' in kwargs:
        recv_until(kwargs['syncmark'])
    return multifun(*args)


def run_multi_sequential(fun, queue, do_all, polarity):
    processed = 0
    results = {}
    while (len(results) < len(queue)):
        print(queue[processed])

        found_cex = False
        return_code = fun(*queue[processed])
        assert (return_code == 10 or return_code == 20)
        is_sat = (return_code == 10)
        results[processed] = is_sat
        found_cex = found_cex or (is_sat == polarity)
        processed += 1
        if not do_all and found_cex: break
    return results

def run_multi_parallel(fun, queue, do_all, polarity, delay=0.01, worker_count=NUM_CPUS):
    results = {}
    with MP_CONTEXT.Pool(worker_count, initializer=set_multifun, initargs=(fun,)) as pool:
        for i, res in enumerate(tqdm(pool.imap(run_multifun_expand, queue), total=len(queue))):
            return_code = res
            assert (return_code == 10 or return_code == 20)
            is_sat = (return_code == 10)
            results[i] = is_sat
    return results

def run_multi_interactive(fun, initial_queue, on_result, worker_count=NUM_CPUS, multipipe: Optional[Multipipe]=None):
    tasks_started = 0
    tasks_done = 0
    result_queue = queue.Queue()
    last_updatetime = datetime.now() - timedelta(seconds=60)
    
    def print_progress(last=False):
        nonlocal last_updatetime
        if last or last_updatetime < datetime.now() - timedelta(seconds=5):
            last_updatetime = datetime.now()
            #print(f'progress: {tasks_done}/{tasks_started}')
            print(f'\rprogress: {tasks_started} started, {tasks_done} done, ({tasks_started-tasks_done} in progress)', end=None if last else '')
        
    def error_callback(ex):
        result_queue.put((None, ex))
            
    def result_callback(args, res):
        result_queue.put((args, res))
        
    if multipipe:
        multipipe.set_n_pipes(worker_count)
    
    with MP_CONTEXT.Pool(worker_count, initializer=set_multifun, initargs=(fun,multipipe)) as pool:
        
        def add_to_queue(args):
            nonlocal tasks_started
            tasks_started += 1
            kwds = {'syncmark': multipipe.syncmarker} if multipipe else {}
            pool.apply_async(run_multifun, args, kwds=kwds,
                             callback=lambda res: result_callback(args, res),
                             error_callback=error_callback)
        
        def add_result(args, res):
            nonlocal tasks_started
            tasks_started += 1
            result_queue.put((args, res))
            
        for job in initial_queue:
            add_to_queue(job)

        print_progress()
        while tasks_done < tasks_started:
            try:
                args, res = result_queue.get(timeout=5)
                if args == None:
                    print('Exception:', res)
                    raise res
                on_result(args, res, add_to_queue, add_result)
                tasks_done += 1
            except queue.Empty:
                pass
            finally:
                print_progress()
        print_progress(last=True)
