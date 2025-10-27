#pragma once

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define sqr(x) ((x)*(x))
#define sign(x) ((x)>=0?1:-1)

//Макросы для получения старшего и младшего байтов в слове (word)
#define LByte(w) ((uint8_t) ((w) & 0xff))
#define HByte(w) ((uint8_t) ((w) >> 8))
