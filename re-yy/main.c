/*
 * main.c
 * Copyright (C) 2019 hzshang <hzshang15@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "stand.h"
#include "flag.tab.h"

extern FILE* yyin;
extern int yydebug;
char input[0x100];
// *CTF{yy_funct10n_1s_h4rd_and_n0_n33d_to_r3v3rs3}
int main(int argc, char *argv[]){
//    yydebug=1;
    printf("give me flag:");
    scanf("%255s",input);
    yyin = fmemopen(input,strlen(input),"r");
    yyparse();
    return 0;
}
