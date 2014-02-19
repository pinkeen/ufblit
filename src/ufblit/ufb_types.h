#ifndef UFB_TYPES_H
#define UFB_TYPES_H

#include <stdint.h>
#include <stdlib.h>


typedef uint8_t UFBuint8;
typedef uint16_t UFBuint16;
typedef uint32_t UFBuint32;
typedef uint64_t UFBuint64;

typedef int8_t UFBint8;
typedef int16_t UFBint16;
typedef int32_t UFBint32;
typedef int64_t UFBint64;


/*
typedef unsigned char UFBuint8;
typedef unsigned short UFBuint16;
typedef unsigned int UFBuint32;
typedef unsigned long UFBuint64;

typedef char UFBint8;
typedef short UFBint16;
typedef int UFBint32;
typedef long UFBint64;
*/


typedef size_t UFBsize;

typedef UFBuint8 UFBbool;

#define UFBfalse 0
#define UFBtrue 1

typedef enum
{
    UFB_IT_UNKNOWN,
    UFB_IT_TGA,
    UFB_IT_PNG,
    UFB_IT_JPEG
} UFB_ImageType;

typedef struct __attribute__((__packed__))
{
    UFBuint8 r;
    UFBuint8 g;
    UFBuint8 b;
    UFBuint8 a;
} UFB_RGBA8888;

typedef struct __attribute__((__packed__))
{
    UFBuint16 rgb;
    UFBuint8 a;
} UFB_RGBA5658;

typedef struct
{
    UFBint16 x;
    UFBint16 y;
    UFBint16 w;
    UFBint16 h;
} UFB_Rect;


#endif // UFB_TYPES_H
