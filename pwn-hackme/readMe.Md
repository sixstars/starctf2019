## hackme  

It's a kernel challenge I made for *CTF2019, the origin source code is `hackme.c`. But I made a real bug, forgetting to check the boundary of index. And I updated a new version, you can download the new attached file at [here](https://drive.google.com/drive/folders/1tL8ADFyAxzsWuqT7axyUItQtzRrHyGzj?usp=sharing).


If you want to try the new version without any hints, plz don't rolling your mouse.  

```
















































```









## Idea
I read a blog at [https://cyseclabs.com/blog/linux-kernel-heap-spray](https://cyseclabs.com/blog/linux-kernel-heap-spray) recently, it described a very powerful way to exploit some kernel vulnerabilities, so I want to use it for my challenge.
  
This challenge just like a normal heap challenge, but is implemented it in kernel space.   

```C
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
```  
the bug is in add and delete functions. At delete function, It only erases the pointer but keeps the old size, and there is no lock at add function. So a basic  exploit as follows:  

- add then delete a large note to keep a big old size at a slot.
- call `add_note`, malloc a small note and trap pc to user space at [A] using `userfault_fd`.  
- start a new thread and edit the victim note_unit, it causes read and write  out of boundary at kernel heap space.  
- search ptmx struct and hijack it.  


```C  
struct note_unit{
    void* ptr;
    uint64_t size;
};
static long add_note(uint32_t idx,void* ptr, uint64_t size){
    void *tmp;
    if(pool[idx].ptr)
        return -1;
    tmp = kmalloc(size,GFP_KERNEL);
    if(!tmp)
        return -1;
    pool[idx].ptr = tmp;
    copy_from_user(tmp,ptr,size);   <== A
    pool[idx].size = size;
    return 0;
}
static long del_note(uint32_t idx){
    if(pool[idx].ptr){
        kfree(pool[idx].ptr);
        pool[idx].ptr = 0;
        return 0;
    }else{
        return -1;
    }
}

```

Expolit example: [exp.c](./exp.c)  
Build kernel guide: https://gist.github.com/chrisdone/02e165a0004be33734ac2334f215380e






