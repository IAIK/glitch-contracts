import networkx as nx
import pickle 
import time
from defines import *

class SafeGraph(nx.DiGraph):
    def __init__(self, orig):
        # initialize graph and get list of initial nodes
        nx.DiGraph.__init__(self, orig)
        self._node_list = []
        for n in orig.nodes():
            n_type = orig.nodes[n]["cell"].type
            if (n_type in REGPORT_TYPES) or (n_type == CONST_TYPE):
                self._node_list.append(n)
        visited = set(self._node_list)
        self._depth = {n: 0 for n in self._node_list}
        # print(visited)
        self._node_list.sort()
        # create a queue for the traversal
        queue = set()
        for q in self._node_list:
            for s in orig.successors(q):
                queue.add(s)
        # special handling for muxes
        queue = queue.union([m for m in orig.nodes() if orig.nodes[m]["cell"].select in visited])
        queue = sorted(list(queue.difference(visited)))
        # make a cache of predecessors including mux selectors
        predecessors = {}
        for n in orig.nodes():
            predecessors[n] = list(orig.predecessors(n))
            cell = orig.nodes[n]["cell"]
            if cell.type == MUX_TYPE:
                predecessors[n].append(cell.select)
        # topological traversal starting from visited nodes
        q_set = set(queue)
        while len(queue) != 0:
            # print(queue)
            nqueue = []
            for q in queue:
                # print("%d: %s %s" % (q, predecessors[q], [p in visited for p in predecessors[q]]))
                if all(map(lambda p: p in visited, predecessors[q])):
                    self._node_list.append(q)
                    self._depth[q] = max(map(lambda p: self._depth[p], predecessors[q])) + 1
                    visited.add(q)
                    q_set.remove(q)
                    for succ in orig.successors(q):
                        if succ not in q_set and succ not in visited:
                            nqueue.append(succ)
                            q_set.add(succ)
                    continue
                nqueue.append(q)
            assert(queue != nqueue)
            queue = nqueue
            assert(len(queue) == len(q_set))
            # print("queue: %6d\r" % len(queue))
        # print("")
        assert(len(orig.nodes()) == len(self._node_list))
        self._cells = {n: orig.nodes[n]["cell"] for n in self._node_list}
        
        self._all_predecessors = {nid: tuple(self._calc_all_predecessors(nid)) for nid in self.nodes}
        all_successors = {nid: list(self.successors(nid)) for nid in self.nodes}
        for nid in self.nodes:
            cell = self.cells[nid]
            if cell.type == MUX_TYPE:
                all_successors[cell.select].append(nid)
        self._all_successors = {nid: tuple(li) for nid, li in all_successors.items()}

    def remove_node(self, n):
        assert(False), "Cannot remove nodes"

    def add_node(self, node_for_adding, **attr):
        assert(False), "Cannot add nodes"

    def remove_edge(self, u, v):
        assert(False), "Cannot remove edges"

    def add_edge(self, u_of_edge, v_of_edge, **attr):
        assert(False), "Cannot add edges"

    def get_depth(self, n):
        return self._depth[n]

    @property
    def nodes(self):
        return self._node_list

    @property
    def cells(self):
        return self._cells

    def write_pickle(self, args):
        t1 = time.time()
        with open(args.safe_graph_path, 'wb') as f:
            pickle.dump(self, f)
        t2 = time.time()
        print("Writing SafeGraph: %.2f" % (t2-t1))

    def _calc_all_predecessors(self, nid):
        ''' calculate all predecessors including mux select '''
        yield from self.predecessors(nid)
        cell = self.cells[nid]
        if cell.type == MUX_TYPE:
            yield cell.select

    def all_predecessors(self, nid):
        ''' predecessors including mux select '''
        return self._all_predecessors[nid]

    def all_successors(self, nid):
        ''' successors including mux select '''
        return self._all_successors[nid]
