#ifndef SCENE_H_
#define SCENE_H_

typedef struct _vector {
	float x, y, z;
} vector;

typedef struct _sphere {
	vector position;
	vector color;
	float radius;
	float reflection;
} sphere;

typedef struct _light {
	vector position;
	vector color;
} light;

typedef struct _scene {
	vector bgcolor;
	vector camera;
	float numSpheres;
	float numLights;
	float antiAliasing;
	float resolution;
	float rayDepth;
	sphere spheres[20];
	light lights[10];
} scene;

typedef struct _ray {
	vector start;
	vector direction;
} ray;

int sceneResolution(scene *s);
scene* sceneLoad();
void sceneFree(scene *s);

#endif /* SCENE_H_ */
