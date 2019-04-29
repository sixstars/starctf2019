## Idea

heap master combined some ideas of 0ctf2018 heapstorm 2 and hitcon2018 baby tcache together.  


It maped a random segment at init. You can edit it and free the pointer in the segment arbitrarily, but there is no show function. And malloc function seems very interesting, the value which malloc returned is never used again. 

```C
void add(){
    printf("size: ");
    uint64_t size = read_num();
    if( size < 0 ){
        puts("Invaild input");
        return;
    }
    malloc(size);
}
```  

## solve
0ctf2018 heapstorm2 introduced a new method to write a heap pointer at arbitrary address using largebin attack when malloc. The bug code is in [https://code.woboq.org/userspace/glibc/malloc/malloc.c.html#3858](https://code.woboq.org/userspace/glibc/malloc/malloc.c.html#3858)  

```C
  if ((unsigned long) (size)
      < (unsigned long) chunksize_nomask (bck->bk))
    {
      fwd = bck;
      bck = bck->bk;
      victim->fd_nextsize = fwd->fd;
      victim->bk_nextsize = fwd->fd->bk_nextsize;
      fwd->fd->bk_nextsize = victim->bk_nextsize->fd_nextsize = victim;  <== no check
    }
```
 
You can find a detailed writeup at [http://eternalsakura13.com/2018/04/03/heapstorm2/](http://eternalsakura13.com/2018/04/03/heapstorm2/).   

Unlike heapstorm2, there is no show function and I chroot the path so you must excute your own shellcode to read flag (no more one gadget), thats means you must  leak some address information before expolit it, so I think it's obvious that you should play with FILE struct.   

At babytache, Angelboy overwrites the stdout flags and IO_write buffer to leak libc. It's a very fresh way to bypass ASLR.  

Here is expolit steps:

#### step 1 
free large chunk to write main_arena unsortbin address to the new segment.  
#### step 2
partial overwrite libc address, point it to stdout flag, use largebin attack(House of Storm) to overwrite stdout flag. if `fp->flag & 0xa00 == 1` and `fp->flag & 0x1000 == 1` then it will leak something when `f->write_base != f->write_ptr`  

You can find FILE code at [https://code.woboq.org/userspace/glibc/libio/fileops.c.html#1218](https://code.woboq.org/userspace/glibc/libio/fileops.c.html#1218)


#### step4 
partial overwrite libc address, point it to stdout write buffer, use largebin attack again to overwrite stdout the last byte of `_IO_write_base` to `'\x00'`.  

Now program will print libc address and our segment address(we have write it to fp->flag) before print menu.  

#### step5
overwrite `_dl_open_hook` just like step 3 & 4, it's a pointer of a function pointer, write our segment address in it.  

#### step6
trigger libc malloc or free error, program will load `_dl_open_hook` to a register and call it. In this libc, the register is rbx. And I find some gadgets in glibc as follows:  

```
0x7FD7D: mov     rdi, [rbx+48h]
         mov     rsi, r13
         call    qword ptr [rbx+40h]

```
Perfect! we control rbx and rdi now! Then jump to setcontext and control all registers.  










