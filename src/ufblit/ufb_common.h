#ifndef UFB_COMMON_H
#define UFB_COMMON_H

#include "ufb_types.h"

#define R565MASK 0xf800
#define G565MASK 0x07e0
#define B565MASK 0x001f

#define RGB888TO565(r, g, b) (((b) >> 3) | (((g) >> 2) << 5) | (((r) >> 3) << 11))

#define RGB565(r, g, b) ((r) << 11 | (g) << 5 | (b))

#define R565(c) (((c) & R565MASK) >> 11)
#define G565(c) (((c) & G565MASK) >> 5)
#define B565(c) (((c) & B565MASK))

void UFB_ExtractFileExt(const char *fname, char *ext, UFBsize bufSz);
UFB_ImageType UFB_GuessImageTypeFromExt(const char *fname);

#endif // UFB_COMMON_H
