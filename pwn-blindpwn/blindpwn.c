#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int bug(){
	char buf[0x20];
	read(0,buf,0x100);
	return 0;
}

void init(){
    setbuf(stdin, 0LL);
    setbuf(stdout, 0LL);
    setbuf(stderr, 0LL);
}

int main(int argc, char const *argv[]){   
	init();
    write(1,"Welcome to this blind pwn!\n",27);
	bug();
    write(1, "Goodbye!\n",9);
	return 0;
}
