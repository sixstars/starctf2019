/*
 * main.c
 * Copyright (C) 2019 hzshang <hzshang15@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define SIZE 0x10000
void* heap_base;
void prog_init(){
    setvbuf(stdin,0,2,0);
    setvbuf(stdout,0,2,0);
    int fd = open("/dev/urandom",0);
    if(fd<0){
        fprintf(stderr,"/dev/urandom: %s\n",strerror(errno));
        exit(1);
    }
    read(fd,&heap_base,8);
    close(fd);
    heap_base = (void*)((uint64_t)heap_base & (uint64_t)0xfffff000);
    if(mmap(heap_base,SIZE,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0)!=heap_base){
        fprintf(stderr,"mmap: %s\n",strerror(errno));
        exit(1);
    }
    malloc(0);
}
uint64_t read_num(){
    char buf[0x10];
    fgets(buf,0x10,stdin);
    return atol(buf);
}
void edit(){
    printf("offset: ");
    uint64_t offset = read_num();
    printf("size: ");
    uint64_t size = read_num();
    if( offset >= SIZE || size > SIZE || offset + size > SIZE ){
        puts("Invaild input");
        return;
    }
    printf("content: ");
    int n=0;
    while(n<size){
        int tmp = read(0,offset+heap_base,size-n);
        if(tmp <0)
            break;
        n +=tmp;
    }
}
void delete(){
    printf("offset: ");
    uint64_t offset = read_num();
    if( offset >= SIZE ){
        puts("Invaild input");
        return;
    }
    free(heap_base+offset);
}
void add(){
    printf("size: ");
    uint64_t size = read_num();
    if( size < 0 ){
        puts("Invaild input");
        return;
    }
    malloc(size);
}
int main(int argc, char *argv[]){
    prog_init();
    while(1){
        puts("=== Heap Master ===");
        puts("1. Malloc");
        puts("2. Edit");
        puts("3. Free");
        printf(">> ");
        int idx = read_num();
        switch(idx){
            case 1:
                add();
                break;
            case 2:
                edit();
                break;
            case 3:
                delete();
                break;
            default:
                puts("Em?");
                exit(0);
        }
    }
    return 0;
}





