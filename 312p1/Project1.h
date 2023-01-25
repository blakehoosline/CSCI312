#ifndef PROJECT1_H
#define PROJECT1_H


//defining binary left and right shift
#ifdef BINARY
#define SRC(x) ((x >> 1) | ((x & 1) << 31))
#define SLC(x) ((x << 1) | ((x >> 31) & 1))
#endif

//defining the hexadecimal left and right shift
#ifdef HEXADECIMAL
#define SRC(x) ((x >> 4) | ((x & 0xF) << 28))
#define SLC(x) ((x << 4) | (x >> 28))
#endif

#endif
