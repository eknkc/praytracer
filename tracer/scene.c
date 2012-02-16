#include "scene.h"
#include "stdlib.h"
#include "stdio.h"

void skipLine(FILE *f)
{
	char c;
	while ((c = fgetc(f)) != EOF)
	{
		if ((c <= '9' && c >= '0') || c == '.' || c == '-')
		{
			fseek(f, -1, SEEK_CUR);
			break;
		}
	}
}

scene* sceneLoad()
{
	int i, numSpheres = 0, numLights = 0, antiAliasing, resolution, recursion;
	FILE *sceneFile = fopen("scene.dat", "r");
	if (!sceneFile) {
		printf("Scene file 'scene.dat' not found!");
		return NULL;
	}

	scene *s = (scene*)malloc(sizeof(scene));

	skipLine(sceneFile);
	(void) fscanf(sceneFile, "%f %f %f", &s->bgcolor.x, &s->bgcolor.y, &s->bgcolor.z);

	skipLine(sceneFile);
	(void) fscanf(sceneFile, "%f %f %f", &s->camera.x, &s->camera.y, &s->camera.z);

	skipLine(sceneFile);
	(void) fscanf(sceneFile, "%d %d %d %d %d", &numSpheres, &numLights, &resolution, &antiAliasing, &recursion);
	s->numSpheres = numSpheres + 0.5f;
	s->numLights = numLights + 0.5f;
	s->resolution = resolution + 0.5;
	s->antiAliasing = antiAliasing + 0.5;
	s->rayDepth = recursion + 0.5;

	skipLine(sceneFile);
	for (i = 0; i < numSpheres; i++)
	{
		(void) fscanf(sceneFile, "%f %f %f", &s->spheres[i].position.x, &s->spheres[i].position.y, &s->spheres[i].position.z);
		(void) fscanf(sceneFile, "%f %f %f", &s->spheres[i].color.x, &s->spheres[i].color.y, &s->spheres[i].color.z);
		(void) fscanf(sceneFile, "%f", &s->spheres[i].radius);
		(void) fscanf(sceneFile, "%f", &s->spheres[i].reflection);
	}

	skipLine(sceneFile);
	for (i = 0; i < numLights; i++)
	{
		(void) fscanf(sceneFile, "%f %f %f", &s->lights[i].position.x, &s->lights[i].position.y, &s->lights[i].position.z);
		(void) fscanf(sceneFile, "%f %f %f", &s->lights[i].color.x, &s->lights[i].color.y, &s->lights[i].color.z);
	}

	fclose(sceneFile);

	return s;
}

int sceneResolution(scene *s)
{
	return (int)s->resolution;
}

void sceneFree(scene *s)
{
	free(s);
}
