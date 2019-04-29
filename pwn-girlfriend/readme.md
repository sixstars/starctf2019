### girlfriend
#### Note
 I long for love, but also like single life. But it's a little difficult to find a girlfriend by playing CTF.

#### idea
we compile glibc-2.29 which teache will [check double free](https://patchwork.ozlabs.org/patch/994064/). But it is also easy to bypass by filling 7 chuncks on it. Now you will double free by fastbin. You can malloc trunck near free_hook, and revise it to system, free a trunck ptr which point to "/bin/sh\x00" will get the shell. By the way, libc is easy to leak by malloc and delete a large chunck.

#### solve
check `hack.py`