/*
 * mutate.h
 * Copyright (C) 2019 hzshang <hzshang15@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef MUTATE_H
#define MUTATE_H
#include <stdint.h>
#define EDGE 3
extern uint32_t Front[][EDGE];
extern uint32_t Behind[][EDGE];
extern uint32_t Left[][EDGE];
extern uint32_t Right[][EDGE];
extern uint32_t Up[][EDGE];
extern uint32_t Down[][EDGE];

void R();
void Ri();
void L();
void Li();
void M();
void Mi();
void U();
void Ui();
void T();
void Ti();
void D();
void Di();
void F();
void Fi();
void W();
void Wi();
void B();
void Bi();

#endif /* !MUTATE_H */
