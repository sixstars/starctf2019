%{
#include "stand.h"
#include "flag.tab.h"
int state_pos;
int yywrap(void){
    state_pos = 1;
    return 1;
}
#define ADJ do{\
    state_pos += yyleng;\
} while(0)

%}

%%
<INITIAL>{
    "*" {ADJ;return STAR;}
    "{" {ADJ;return LBRACK;}
    "}" {ADJ;return RBRACK;}
    "a" {ADJ;return A;}
    "b" {ADJ;return B;}
    "c" {ADJ;return C;}
    "d" {ADJ;return D;}
    "e" {ADJ;return E;}
    "f" {ADJ;return F;}
    "g" {ADJ;return G;}
    "h" {ADJ;return H;}
    "i" {ADJ;return I;}
    "j" {ADJ;return J;}
    "k" {ADJ;return K;}
    "l" {ADJ;return L;}
    "m" {ADJ;return M;}
    "n" {ADJ;return N;}
    "o" {ADJ;return O;}
    "p" {ADJ;return P;}
    "q" {ADJ;return Q;}
    "r" {ADJ;return R;}
    "s" {ADJ;return S;}
    "t" {ADJ;return T;}
    "u" {ADJ;return U;}
    "v" {ADJ;return V;}
    "w" {ADJ;return W;}
    "x" {ADJ;return X;}
    "y" {ADJ;return Y;}
    "z" {ADJ;return Z;}
    
    "0" {ADJ;return ZERO;}
    "1" {ADJ;return ONE;}
    "2" {ADJ;return TWO;}
    "3" {ADJ;return THREE;}
    "4" {ADJ;return FOUR;}
    "5" {ADJ;return FIVE;}
    "6" {ADJ;return SIX;}
    "7" {ADJ;return SEVEN;}
    "8" {ADJ;return EIGHT;}
    "9" {ADJ;return NINE;}

    "_" {ADJ;return UNDERSCORE;}
    "C" {ADJ;return UP_C;}
    "T" {ADJ;return UP_T;}
    "F" {ADJ;return UP_F;}
}
. {
 ADJ; fprintf(stderr,"input illegal!\n"); exit(1);
}

