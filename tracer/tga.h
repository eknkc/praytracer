#ifndef TGA_H_
#define TGA_H_

#include "stdio.h"
#include "scene.h"
#include "functions.h"

typedef FILE TGAFILE;

TGAFILE* openTGA(scene *curscene);
void writeTGAColor(TGAFILE *out, scene *curscene, int x, int y, float red, float green, float blue);
void closeTGA(TGAFILE *out);

#endif /* TGA_H_ */
