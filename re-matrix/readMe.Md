## matr1x

### Idea
zzj wanna design a magic cube challenge, but it's too easy.  
Then I added some code obfuscation and changed his check funtion.  

the code confusion module is a modified version from [https://yurichev.com/blog/58/](https://yurichev.com/blog/58/), blogger implemented it on Window PE, I make it works on x86 platform.  


There is five kinds of obfuscation.  

#### PUSH JMP INSTEAD OF CALL
`call func` to  
 
```
call . + $5
pop eax
add eax,0x0a
push eax
jmp func
```

#### POP JMP INSTEAD OF RET
`ret` to  

```
pop ebx
jmp ebx
```

plz check the [source code](./tcc-0.9.25.diff) for other methods. 
#### NOISE AROUND OPI
#### NOISE AFTER CALL
#### LOAD


