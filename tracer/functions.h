#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define MIN(a,b) ((a)>(b)?(b):(a))

#include "math.h"
#include "scene.h"

vector vectormake(float ix, float iy, float iz);

float dot(vector first, vector second);

vector smul(vector v, float s);

vector add(vector first, vector second);

vector sub(vector first, vector second);

float norm(vector v);

vector unit(vector v);

ray raymake(vector start, vector end);

float spherehit(sphere s, ray r);

#endif /* FUNCTIONS_H_ */
