# encoding: utf8

"""
python3.7 sorted func by 2 elements
"""

class T(object):
    def __init__(self, a, b):
        self._a = a
        self._b = b

    def __repr__(self):
        return f'T({self._a}, {self._b})'

T1 = T(1, 300)
T2 = T(2, 200)
T3 = T(3, 200)
T4 = T(3, 100)
T5 = T(2, 100)


def func_cmp(x,y):
    if x._a != y._a:
        return x._a - y._a # ascending order by a
    else:
        return y._b - x._b # descending order by b

from functools import cmp_to_key

l = [T1, T2, T3, T4, T5]
print(l)
sorted_l = sorted(l, key=cmp_to_key(func_cmp))
print(sorted_l)
