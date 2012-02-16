#include "../tracer/tracer.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

#ifdef SEQUENTIAL

int main(int argc, char *argv[])
{
	initTracer();
	scene *curscene = sceneLoad();
	TGAFILE *tga = openTGA(curscene);

	clock_t start = clock();

	int i, j;

	for (j = 0; j < (int)curscene->resolution; j++)
	{
		printf("Scanning horizontal line %d.\n", j);

		for (i = 0; i < (int)curscene->resolution; i++)
		{
			vector color = pixelraytrace(curscene, i, j);
			writeTGAColor(tga, curscene, i, j, color.x, color.y, color.z);
		}
	}

	clock_t end = clock();

	printf("Entire process took %.4f seconds.\n", ((double)(end - start)) / CLOCKS_PER_SEC);

	sceneFree(curscene);
	closeTGA(tga);

	return 0;
}

#endif
