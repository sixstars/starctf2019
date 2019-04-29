%{
#include "stand.h"
#include "util.h"
int yylex();
void yyerror(char const *msg){
    fprintf(stderr,"%s\n",msg);
    exit(1);
}

%}
%token
    STAR LBRACK RBRACK ZERO ONE TWO THREE FOUR FIVE SIX SEVEN EIGHT NINE
    A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
    UNDERSCORE UP_C UP_T UP_F
%%
program:
     parse_done{
        if(!memcmp(result,cmp,0xa0)){
            printf("Congratulations!\n");
        }else{
            printf("try again!\n");
        }
    };
parse_done:
    flag_title {pc=0;memcpy(buffer,append,0x10);}words flag_end{}
    ;

flag_title:
    STAR UP_C UP_T UP_F LBRACK {}
    ;

flag_end:
    RBRACK {}
    ;

word_with_underscore:
    UNDERSCORE{ pc=0;memcpy(buffer,append,0x10);} word{encrypt(buffer);}
    ;

word_with_underscore_list:
    {}
    |word_with_underscore word_with_underscore_list {}
    ;

words:
    word {encrypt(buffer);} word_with_underscore_list{ }
    ;

word:
    letter {}
    |letter  word {}
    ;


letter:
    A { buffer[pc++]=box[0];}
    |B{ buffer[pc++]=box[1];}
    |C{ buffer[pc++]=box[2];}
    |D{ buffer[pc++]=box[3];}
    |E{ buffer[pc++]=box[4];}
    |F{ buffer[pc++]=box[5];}
    |G{ buffer[pc++]=box[6];}
    |H{ buffer[pc++]=box[7];}
    |I{ buffer[pc++]=box[8];}
    |J{ buffer[pc++]=box[9];}
    |K{ buffer[pc++]=box[10];}
    |L{ buffer[pc++]=box[11];}
    |M{ buffer[pc++]=box[12];}
    |N{ buffer[pc++]=box[13];}
    |O{ buffer[pc++]=box[14];}
    |P{ buffer[pc++]=box[15];}
    |Q{ buffer[pc++]=box[16];}
    |R{ buffer[pc++]=box[17];}
    |S{ buffer[pc++]=box[18];}
    |T{ buffer[pc++]=box[19];}
    |U{ buffer[pc++]=box[20];}
    |V{ buffer[pc++]=box[21];}
    |W{ buffer[pc++]=box[22];}
    |X{ buffer[pc++]=box[23];}
    |Y{ buffer[pc++]=box[24];}
    |Z{ buffer[pc++]=box[25];}
    |ZERO{buffer[pc++]=box[26];}
    |ONE{buffer[pc++]=box[27];}
    |TWO{buffer[pc++]=box[28];}
    |THREE{buffer[pc++]=box[29];}
    |FOUR{buffer[pc++]=box[30];}
    |FIVE{buffer[pc++]=box[31];}
    |SIX{buffer[pc++]=box[32];}
    |SEVEN{buffer[pc++]=box[33];}
    |EIGHT{buffer[pc++]=box[34];}
    |NINE{buffer[pc++]=box[35];}
    ;

