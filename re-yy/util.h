/*
 * util.h
 * Copyright (C) 2019 hzshang <hzshang15@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef UTIL_H
#define UTIL_H
extern int pc;
extern unsigned char buffer[];
extern unsigned char box[];
extern unsigned char append[];
extern unsigned char result[];
extern unsigned long long cmp[];
void encrypt(unsigned char *buf);
#endif /* !UTIL_H */
