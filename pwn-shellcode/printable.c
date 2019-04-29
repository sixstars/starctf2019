/*
 * printable.c
 * Copyright (C) 2019 vam <jpwan21@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

int check(char *shellcode){
   char *s = "ZZJ loves shell_code,and here is a gift:\x0f\05 enjoy it!\n";
   char *i=shellcode, *j;
    
   while(*i){
       for(j=s;*j;j++)
           if(*j==*i)
               break;
       if(*j==0)
           return 0;
       i++;
   }   
   return 1;
}

void init(){
   setbuf(stdin, 0LL);
   setbuf(stdout, 0LL);
   setbuf(stderr, 0LL);
   alarm(0x3c);
}
int main(){
   init();
   char *shellcode =(char *)mmap(NULL, 0x1000,PROT_READ|PROT_WRITE|PROT_EXEC,
           MAP_PRIVATE|MAP_ANON,0,0);
   printf("give me shellcode, plz:\n");
   read(0,shellcode, 0x200);
   if(check(shellcode)){
        void (*run_shellcode)() = shellcode;
        run_shellcode();
   }else{
       printf("wrong shellcode!");
       exit(0);
   }
   return 0;
}



