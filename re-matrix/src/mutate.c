/*
 * mutate.c
 * Copyright (C) 2019 hzshang <hzshang15@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "mutate.h"

void rot(uint32_t p[][EDGE]){
    int temp = 0;
    temp = p[0][0];
    p[0][0] = p[2][0];
    p[2][0] = p[2][2];
    p[2][2] = p[0][2];
    p[0][2] = temp;
    temp = p[0][1];
    p[0][1] = p[1][0];
    p[1][0] = p[2][1];
    p[2][1] = p[1][2];
    p[1][2] = temp;
    return;
}
void roti(uint32_t p[][EDGE]){
    int temp = 0;
    temp = p[0][0];
    p[0][0] = p[0][2];
    p[0][2] = p[2][2];
    p[2][2] = p[2][0];
    p[2][0] = temp;
    temp = p[0][1];
    p[0][1] = p[1][2];
    p[1][2] = p[2][1];
    p[2][1] = p[1][0];
    p[1][0] = temp;
    return;
}
void R(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Front[i][2];
        Front[i][2] = Down[i][2];
        Down[i][2] = Behind[i][2];
        Behind[i][2] = Up[i][2];
        Up[i][2] = temp;
    }
    rot(Right);
    return;
}
void Ri(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Front[i][2];
        Front[i][2] = Up[i][2];
        Up[i][2] = Behind[i][2];
        Behind[i][2] = Down[i][2];
        Down[i][2] = temp;
    }
    roti(Right);
    return;
}
void L(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Front[i][0];
        Front[i][0] = Up[i][0];
        Up[i][0] = Behind[i][0];
        Behind[i][0] = Down[i][0];
        Down[i][0] = temp;
    }
    rot(Left);
    return;
}
void Li(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Front[i][0];
        Front[i][0] = Down[i][0];
        Down[i][0] = Behind[i][0];
        Behind[i][0] = Up[i][0];
        Up[i][0] = temp;
    }
    roti(Left);
    return;
}
void M(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Front[i][1];
        Front[i][1] = Down[i][1];
        Down[i][1] = Behind[i][1];
        Behind[i][1] = Up[i][1];
        Up[i][1] = temp;
    }
    return;
}
void Mi(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Up[i][1];
        Up[i][1] = Behind[i][1];
        Behind[i][1] = Down[i][1];
        Down[i][1] = Front[i][1];
        Front[i][1] = temp;
    }
    return;
}
void U(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Front[0][i];
        Front[0][i] = Right[0][i];
        Right[0][i] = Behind[2][EDGE-i-1];
        Behind[2][EDGE-i-1] = Left[0][i];
        Left[0][i] = temp;
    }
    rot(Up);
    return;
}
void Ui(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Front[0][i];
        Front[0][i] = Left[0][i];
        Left[0][i] = Behind[2][EDGE-i-1];
        Behind[2][EDGE-i-1] = Right[0][i];
        Right[0][i] = temp;
    }
    roti(Up);
    return;
}

void T(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Front[1][i];
        Front[1][i] = Right[1][i];
        Right[1][i] = Behind[1][EDGE-i-1];
        Behind[1][EDGE-i-1] = Left[1][i];
        Left[1][i] = temp;
    }
    return;
}
void Ti(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Front[1][i];
        Front[1][i] = Left[1][i];
        Left[1][i] = Behind[1][EDGE-i-1];
        Behind[1][EDGE-i-1] = Right[1][i];
        Right[1][i] = temp;
    }
    return;
}
void D(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Front[2][i];
        Front[2][i] = Left[2][i];
        Left[2][i] = Behind[0][EDGE-i-1];
        Behind[0][EDGE-i-1] = Right[2][i];
        Right[2][i] = temp;
    }
    rot(Down);
    return;
}
void Di(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Front[2][i];
        Front[2][i] = Right[2][i];
        Right[2][i] = Behind[0][EDGE-i-1];
        Behind[0][EDGE-i-1] = Left[2][i];
        Left[2][i] = temp;
    }
    roti(Down);
    return;
}
void F(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Up[2][i];
        Up[2][i] = Left[EDGE-i-1][2];
        Left[EDGE-i-1][2] = Down[0][EDGE-i-1];
        Down[0][EDGE-i-1] = Right[i][0];
        Right[i][0] = temp;
    }
    rot(Front);
    return;
}
void Fi(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Up[2][i];
        Up[2][i] = Right[i][0];
        Right[i][0] = Down[0][EDGE-i-1];
        Down[0][EDGE-i-1] = Left[EDGE-i-1][2];
        Left[EDGE-i-1][2] = temp;
    }
    roti(Front);
    return;
}
void W(){
    int temp = 0;
    int i;
    for (i = 0;i<EDGE;i++){
        temp = Up[1][i];
        Up[1][i] = Right[i][1];
        Right[i][1] = Down[1][EDGE-1-i];
        Down[1][EDGE-1-i] = Left[i][1];
        Left[i][1] = temp;
    }
    return;
}
void Wi(){
    int temp = 0;
    int i;
    for (i = 0;i<EDGE;i++){
        temp = Up[1][i];
        Up[1][i] = Left[i][1];
        Left[i][1] = Down[1][EDGE-1-i];
        Down[1][EDGE-i-1] = Right[i][1];
        Right[i][1] = temp;
    }
    return;
}
void B(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Up[0][i];
        Up[0][i] = Right[i][2];
        Right[i][2] = Down[2][EDGE-i-1];
        Down[2][EDGE-i-1] = Left[EDGE-i-1][0];
        Left[EDGE-i-1][0] = temp;
    }
    rot(Behind);
    return;
}
void Bi(){
    int temp = 0;
    int i;
    for(i = 0;i<EDGE;i++){
        temp = Up[0][i];
        Up[0][i] = Left[EDGE-i-1][0];
        Left[EDGE-i-1][0] = Down[2][EDGE-i-1];
        Down[2][EDGE-i-1] = Right[i][2];
        Right[i][2] = temp;
    }
    roti(Behind);
    return;
}

