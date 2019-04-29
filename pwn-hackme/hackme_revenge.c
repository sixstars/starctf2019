/*
 * hello.c
 * Copyright (C) 2019 hzshang <hzshang15@gmail.com>
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include<linux/slab.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("Dual BSD/GPL");
//MODULE_AUTHOR("");
struct note_unit{
    void* ptr;
    uint64_t size;
};
#define SIZE 0x20
struct note_unit pool[SIZE];
#define ADD 0x30000
#define DEL 0x30001
#define EDIT 0x30002
#define SHOW 0x30003

static long add_note(uint32_t idx,void* ptr, uint64_t size){
    void *tmp;
    if(idx < 0 || idx >= SIZE ||pool[idx].ptr)
        return -1;
    tmp = kmalloc(size,GFP_KERNEL);
    if(!tmp)
        return -1;
    pool[idx].ptr = tmp;
    copy_from_user(tmp,ptr,size);
    pool[idx].size = size;
    return 0;
}
static long del_note(uint32_t idx){
    if(idx < 0 || idx >= SIZE)
        return -1;
    if(pool[idx].ptr){
        kfree(pool[idx].ptr);
        pool[idx].ptr = 0;
        return 0;
    }else{
        return -1;
    }
}

static long show_note(uint32_t idx,void* ptr,uint64_t size,uint64_t pc){
    if(idx < 0 || idx >= SIZE)
        return -1;
    if(!pool[idx].ptr ||  pc + size > pool[idx].size ){
        return -1;
    }
    copy_to_user(ptr,pool[idx].ptr+pc,size);
    return 0;
}

static long edit_note(uint32_t idx,void* ptr,uint64_t size,uint64_t pc){
    if(idx < 0 || idx >= SIZE)
        return -1;
    if(!pool[idx].ptr ||  pc + size > pool[idx].size ){
        return -1;
    }
    copy_from_user(pool[idx].ptr+pc,ptr,size);
    return 0;
}
static long hackme_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
    union {
        struct {uint32_t idx; void* ptr; uint64_t size; } add;
        struct {uint32_t idx; } del;
        struct {uint32_t idx; void* ptr; uint64_t size; uint64_t pc; } edit;
        struct {uint32_t idx; void* ptr; uint64_t size; uint64_t pc; } show;
    } u;
    long ret;
    copy_from_user(&u,(void*)arg,sizeof(u));
    switch (cmd){
        case ADD:
            ret  = add_note(u.add.idx,u.add.ptr,u.add.size);
            break;
        case DEL:
            ret = del_note(u.del.idx);
            break;
        case EDIT:
            ret = edit_note(u.edit.idx,u.edit.ptr,u.edit.size,u.edit.pc);
            break;
        case SHOW:
            ret = show_note(u.show.idx,u.show.ptr,u.show.size,u.show.pc);
            break;
        default:
            ret = -1;
            break;
    }
    return ret;
}
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open =      NULL,
    .release =   NULL,
    .read =      NULL,
    .write =     NULL,
    .unlocked_ioctl = hackme_ioctl
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = "hackme",
    .fops = &fops
};

int hackme_init(void)
{
    printk(KERN_INFO "Welcome to *CTF kernel challenge\n");
    misc_register(&misc);
    return 0;
}

void hackme_exit(void)
{
    printk(KERN_INFO "Goodbye hacker\n");
    misc_deregister(&misc);
}
module_init(hackme_init);
module_exit(hackme_exit);




