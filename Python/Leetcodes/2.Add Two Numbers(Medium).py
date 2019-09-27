# LeetCode:2.Add Two Numbers(Medium)
'''
You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Example

Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
Explanation: 342 + 465 = 807.
'''
# My Answer:

'''
def add(a,b):
    lena = len(a)
    lenb = len(b)
    if (lena > lenb):
        c,d=a,b[:]
        delta = lena - lenb
        length = lena
    else:
        c, d = b, a[:]
        delta = lenb - lena
        length = lenb
    d += [0]*delta

    e = []
    carry = 0
    for i in range(length):
        v = c[i] + d[i] + carry
        if (v >= 10):
            carry = 1
            e.append(v - 10)
        else:
            carry = 0
            e.append(v)
    if (carry == 1):
        e.append(1)

    return e
'''

def add(a, b):
    if a == []:a = [0]
    if b == []:b = [0]
    if len(a) > len(b):a,b = b,a
    l1 = len(a)
    l2 = len(b)
    b[0] = a[0] + b[0]
    for i in range(1,l1):
        b[i] = a[i] + b[i] + b[i-1]//10
        b[i-1] = b[i-1] % 10
    for j in range(l1,l2):
        b[j] = b[j] + b[j-1]//10
        b[j-1] = b[j-1] % 10
    if b[-1] > 9:
        b.append(b[-1]//10)
        b[-2] = b[-2] % 10
    while (b[-1] == 0) and (not b[-1] is b[0]): del b[-1]
    return b


def check(a,b,c):
    actual = add(a[:], b[:])
    if (c != actual):
        print("ERROR: expect %s + %s = %s, actual %s" % (a, b, c, actual))
    else:
        print("PASS: %s + %s = %s" % (a, b, c))

check([], [], [0])
check([0],[0], [0])
check([0,1], [1], [1,1])
check([2,4,3],[5,6,4],[7,0,8])
check([4],[6],[0,1])
