import functools
import os
import sys
import argparse
import hashlib

from CircuitGraph import CONST_TO_BIT


def ap_check_file_exists(file_path):
    if not os.path.isfile(file_path):
        raise argparse.ArgumentTypeError("File '%s' does not exist" % file_path)
    return file_path


def ap_check_dir_exists(file_path):
    dir_path = os.path.dirname(os.path.abspath(file_path))
    if not os.path.isdir(dir_path):
        raise argparse.ArgumentTypeError("Directory '%s' does not exist" % dir_path)
    return file_path


def check_dir_exists(dir_path):
    if not os.path.exists(dir_path):
        raise argparse.ArgumentTypeError("Directory '%s' does not exist" % dir_path)


def ap_check_positive(num_str):
    try:
        val = int(num_str)
        if val <= 0: raise ValueError
    except ValueError:
        raise argparse.ArgumentTypeError("'%s' is not a positive integer value" % num_str)
    return val


def file_hash(file_path):
    BUF_SIZE = 64 * 1024
    hasher = hashlib.sha256()
    with open(file_path, 'rb') as f:
        while True:
            data = f.read(BUF_SIZE)
            if not data: break
            hasher.update(data)
    return hasher.hexdigest()


def status_to_exit(status):
    if os.WIFSIGNALED(status):
        return -os.WTERMSIG(status)
    elif os.WIFEXITED(status):
        return os.WEXITSTATUS(status)
    elif os.WIFSTOPPED(status):
        return -os.WSTOPSIG(status)
    else:
        raise Exception("Unexpected status code")


def ints_as_str(lst):
    return " ".join(map(str, lst))


class ImpTree:
    def __init__(self, leaks=[]):
        self.parent = None
        self.leaks = leaks
        self.children = []
        
    def add_child(self, child):
        self.children.append(child)
        child.parent = self
        
    def add_leak(self, leak):
        self.leaks.append(leak)
        
    def to_str_tree(self, level=0):
        res = f'{"  "*level}{self.leaks}\n'
        for child in self.children:
            res += child.to_str_tree(level+1)
        return res
        
    def __str__(self, level=0):
        return self.to_str_tree()

    def to_list(self):
        yield from self.leaks
        for child in self.children:
            yield from child.to_list()
