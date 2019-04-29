/*
 * quicksort.c
 * Copyright (C) 2019 vam <jpwan21@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void init(){
    setbuf(stdin, 0LL);
    setbuf(stdout, 0LL);
    setbuf(stderr, 0LL);
    //alarm(0x3c);
}

void menu(){
   puts("======================");
   puts("1.Add a girl's info");
   puts("2.Show info");
   puts("3.Edit info");
   puts("4.Call that girl!");
   puts("5.Exit lonely.");
   puts("======================");
   printf("Input your choice:");
}

typedef struct Girl{
    char *name;
    int name_size;
    char phone[12];
} girl;

#define poollength 100
girl *pool[poollength];
int length = 0;

void addinfo(){
    if(length>poollength){
        puts("Enough!");
    }
    pool[length] = (girl*)malloc(sizeof(girl));
    puts("Please input the size of girl's name");
    int size;
    scanf("%d",&size);
    pool[length]->name_size = size;
    pool[length]->name = (char*)malloc(size);

    puts("please inpute her name:");
    read(0,pool[length]->name,size);
    puts("please input her call:");
    read(0,pool[length]->phone,12);
    pool[length]->phone[11] = '\x00'; //add offby null
    puts("Done!");
    length++;
    return;
}

void editinfo(){
     puts("Programmer is tired, delete it and add a new info.");
}

void showinfo(){
    puts("Please input the index:");
    int index;
    scanf("%d",&index);
    
    if(pool[index]){
        puts("name:");
        puts(pool[index]->name);
        puts("phone:");
        puts(pool[index]->phone);
    }
    puts("Done!");
    return;
}

void call_her(){
    puts("Be brave,speak out your love!");
    puts("");
    puts("Please input the index:");
    int index;
    scanf("%d",&index);
    if(index<0 || index>99)
        exit(0);
    if(pool[index]){
      free(pool[index]->name);
    }
    srand((unsigned)time(0));
    int ran_num=rand() % 10;
    if(ran_num<2)
       puts("Now she is your girl friend!");
    else
       puts("Oh, you have been refused.");
    puts("Done!");
    return;
}

void babyheap(){
    
    puts("Do you wanna a girl friend?");
    puts("Maybe she is hidden in the heap!");
    int choice;
    while(1){
        menu();
        scanf("%d", &choice);
        getchar();
        switch(choice){
            case 1:
                addinfo();
                break;
            case 2:
                showinfo();
                break;
            case 3:
                editinfo();
                break;
            case 4:
                call_her();
                break;
            case 5:
                puts("Goodbye~");
                exit(0);
            default:
                puts("Wrong choice!");
        }
    }
    return;
}
int main() {
    init();
    babyheap();
    return 0; 
}
/*
1. unsorted bin, leak libc
2. double free;
   可以创造很多, 小块做缓冲;
*/
