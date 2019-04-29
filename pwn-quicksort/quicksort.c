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
void swap(int *x,int *y){
   int temp;
   temp=*x;
   *x=*y;
   *y=temp;
}
int Partition(int *a, int l, int r){
    int i=l, j=l;
    for(; j<r; j++){
        if(a[j]<a[r])
            swap(&a[i++], &a[j]);
    }
    swap(&a[r], &a[i]);
    return i;
}
void QuickSort(int* a, int l, int r){
    if(l>=r) return;
    int mid= Partition(a, l, r);
    QuickSort(a, l, mid-1);
    QuickSort(a, mid+1, r);
    
}

void init(){
    setbuf(stdin, 0LL);
    setbuf(stdout, 0LL);
    setbuf(stderr, 0LL);
    alarm(0x3c);
}

void sort_func(){
    char s,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15;
    s=s1=s2=s3=s4=s5=s6=s7=s8=s9=s10=s11=s12=s13=s14=s15='\x00';
    int size=0;
    puts("how many numbers do you want to sort?");
    scanf("%d", &size);
    getchar();
    int *a = (int*)malloc(size*sizeof(int));
     
    for(int i=0; i<size; i++){
        printf("the %dth number:", i+1);
        //memset(&s,'\x00',0x10);
        gets(&s);
        a[i]=atoi(&s);
    }
    QuickSort(a, 0, size-1);
    puts("Here is the result:");
    for(int i=0; i<size; i++){
        printf("%d ",a[i]);
    }
    puts("");
    free(a);
}

int main() {
    init();
    sort_func();
    return 0; 
}
