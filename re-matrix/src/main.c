/*
 * rubix.c
 * Copyright (C) 2019 vam <jpwan21@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include "mutate.h"
#include "matrix.h"

int run(char *cmd){
    int length = strlen(cmd);
    int i;
    for(i=0;i<length;i++){
        switch(cmd[i]){
            case 0x10:R() ;break;
            case 0x15:Ri();break;
            case 0x11:L() ;break;
            case 0x14:Li();break;
            case 0x12:M() ;break;
            case 0x13:Mi();break;

            case 0x20:U() ;break;
            case 0x25:Ui();break;
            case 0x21:T() ;break;
            case 0x24:Ti();break;
            case 0x22:D() ;break;
            case 0x23:Di();break;

            case 0x30:F() ;break;
            case 0x35:Fi();break;
            case 0x31:W() ;break;
            case 0x34:Wi();break;
            case 0x32:B() ;break;
            case 0x33:Bi();break;
            default :return 1;
        }
    }
    return 0;
}
#ifdef DEBUG
void print(char* name,uint32_t p[][EDGE]){
    printf("uint32_t %s[EDGE][EDGE]={\n",name);
    for(int i = 0;i<EDGE;i++){
        printf("    {0x%08x, 0x%08x, 0x%08x},\n",p[i][0],p[i][1],p[i][2]);
    }
    printf("};\n");
    return;
}
void print_all(){
    print("Front",Front);
    print("Behind",Behind);
    print("Left",Left);
    print("Right",Right);
    print("Up",Up);
    print("Down",Down);
    return;
}
// MmTtBbUuDdLlWwRrFf

int reverse(char*src,char* dst){
    int length = strlen(src);
    for(int i=0;i<length;i++){
        dst[length-1-i] = (src[i]&0xf0)|(5-src[i]&0xf);
    }
    dst[length]=0;
    return 0;
}
#endif
int check(){
    int ans = 1;
    int i=0;
    for(;i<6 && ans;i++){
        uint32_t* ptr = (uint32_t*)surfaces[i];
        ans &= ptr[0] + ptr[2] + ptr[4] + ptr[6] + ptr[8] == surfaces_m[i][0];
        ans &= ptr[1] + ptr[3] + ptr[4] + ptr[5] + ptr[7] == surfaces_m[i][1];
    }
    return ans;
}
// RLM UTD FWB 
// flag: *CTF{7h1S_Cu63_is_m4g1c}
// 1410133512231421321513231135352230332325242023123424201432232424113535203414212432243232321515212230151132122233221233211035322014201225251320153530321020123511
int main(){
    int i,j;
#ifdef DEBUG
    const char* opt="\x10\x11\x12\x13\x14\x15\x20\x21\x22\x23\x24\x25\x30\x31\x32\x33\x34\x35";
    srand(time(0));
    char op[0x51];
    char rop[0x51];
    for(i=0;i<0x50;i++){
        op[i] = opt[rand()%18];
    }
    op[0x50]=0;
    run(op);
    print_all();
    printf("Mutate ops:");
    for(i=0;i<0x50;i++){
        printf("%02x",op[i]);
    }
    puts("");
    reverse(op,rop);
    printf("Reverse ops:");
    for(i=0;i<0x50;i++){
        printf("%02x",rop[i]);
    }
    puts("");
    run(rop);
#else
    char op_raw[0x100];
    char op[0x51];
    printf("Input your key:");
    scanf("%255s",op_raw);
    int len = strlen(op_raw);
    for(i=0;i<len/2;i++){
        op[i] = ((op_raw[2*i]-'0')<<4)|(op_raw[2*i+1]-'0');
    }
    op[len/2] = 0;
    run(op);
#endif
    if(check()){
        uint32_t flag[0x7];
        uint32_t *ptr;
        uint32_t *ptr_s;
        memset(flag,0,sizeof(flag));
        for(i=0;i<6;i++){
            uint32_t reg=0;
            ptr = (uint32_t*)surfaces[i];
            ptr_s = (uint32_t*)surfaces_s[i];
            for(j=0;j<9;j++){
                reg+=ptr[j]*ptr_s[j];
            }
            flag[i] = reg;
        }
        printf("Here is your flag: %s\n",(char*)flag);
    }else{
        puts("Try again!");
    }
    return 0;
}

