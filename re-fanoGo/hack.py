#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2019 vam <jpwan21@gmail.com>

from pwn import *
context.log_level="debug"
pwn_file="./main"
elf=ELF(pwn_file)

if len(sys.argv)==1:
    r=process(pwn_file)
    pid=r.pid
else:
    r=remote("34.92.37.22", 10001)
    pid=0

def debug():
    log.debug("process pid:%d"%pid)
    pause()

with open("key.txt","r") as f:
    payload = f.read()
    print(payload)
    f.close()
print [ord(x) for x in payload]

debug()
r.sendafter(":",payload)
r.recv()
r.interactive()
