import os
import string
from hashlib import sha256
from pwn import *

context.log_level='debug'

def dopow():
    chal = c.recvline()
    post = chal[12:28]
    tar = chal[33:-1]
    c.recvuntil(':')
    found = iters.bruteforce(lambda x:sha256(x+post).hexdigest()==tar, string.ascii_letters+string.digits, 4)
    c.sendline(found)


def doxor(ss,dd):
    res = ''
    for i in range(8):
        res += chr(ord(ss[i])^dd[i])
    return res

def tonum(x):
    return (u32(x[:4])<<32)+u32(x[4:])

def doout(stats):
    print '{',
    for i in range(3):
        print '{',
        for j in range(6):
            print stats[i*6+j],
            if j!=5:
                print ',',
        print '}',
        if i!=2:
            print ',',
    print '}'

#c = remote('127.0.0.1', 10001)
c = remote('34.92.185.118', 10001)
dopow()
diffs = [[0,0,0,0,0x80,0x80,0,0], [0x80,0x80,0,0,0x80,0x80,0,0], [0,0,0,2,0,0,0,2]]
statsp0 = []
statsc0 = []
statsp1 = []
statsc1 = []
for diff in diffs:
    for i in range(6):
        pt0 = os.urandom(8)
        pt1 = doxor(pt0, diff)
        c.sendlineafter('(hex): ', pt0.encode('hex'))
        ct0 = c.recvline()[:16]
        ct0 = ct0.decode('hex')
        c.sendlineafter('(hex): ', pt1.encode('hex'))
        ct1 = c.recvline()[:16]
        ct1 = ct1.decode('hex')
        statsp0.append(tonum(pt0))
        statsc0.append(tonum(ct0))
        statsp1.append(tonum(pt1))
        statsc1.append(tonum(ct1))

c.sendlineafter('(hex): ', 'go')
c.recvuntil('flag:\n')
cflag = c.recvline()

doout(statsp0)
doout(statsc0)
doout(statsp1)
doout(statsc1)

print cflag
