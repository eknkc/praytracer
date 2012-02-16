#include "functions.h"

vector vectormake(float ix, float iy, float iz)
{
	vector v;
	v.x = ix; v.y = iy; v.z = iz;
	return v;
}

float dot(vector first, vector second)
{
	return first.x * second.x + first.y * second.y + first.z * second.z;
}

vector smul(vector v, float s)
{
	vector res;
	res.x = v.x * s; res.y = v.y * s; res.z = v.z * s;
	return res;
}

vector add(vector first, vector second)
{
	vector res;
	res.x = first.x + second.x;
	res.y = first.y + second.y;
	res.z = first.z + second.z;
	return res;
}

vector sub(vector first, vector second)
{
	vector res;
	res.x = first.x - second.x;
	res.y = first.y - second.y;
	res.z = first.z - second.z;
	return res;
}

float norm(vector v)
{
	return sqrt(dot(v, v));
}

vector unit(vector v)
{
	vector res;
	float n = norm(v);
	res.x = v.x / n;
	res.y = v.y / n;
	res.z = v.z / n;
	return res;
}

ray raymake(vector start, vector end)
{
	ray res;
	res.start = start;
	res.direction = unit(sub(end, start));
	return res;
}

float spherehit(sphere s, ray r)
{
	vector path = sub(s.position, r.start);
	float v = dot(path, r.direction);

	if (v < 0)
		return -1.0;

	float disc = pow(s.radius, 2.0f) - (dot(path, path) - pow(v, 2.0f));

	if (disc < 0)
		return -1.0;

	return v - sqrt(disc);
}
