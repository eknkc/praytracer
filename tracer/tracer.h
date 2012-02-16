#ifndef TRACER_H_
#define TRACER_H_

#include "functions.h"
#include "tga.h"
#include "stdio.h"

void initTracer();
vector pixelraytraceaa(scene *curscene, int x, int y, int aastep);
vector pixelraytrace(scene *curscene, int x, int y);

#endif /* TRACER_H_ */
