#include "mpi.h"
#include "../tracer/tracer.h"
#include "stdlib.h"
#include "math.h"

//#define PIPELINEMAPPING

#ifdef PIPELINEMAPPING

int main(int argc, char *argv[])
{
	initTracer();

	int i;
	int rank, size;
	scene *curscene;
	int res;
	TGAFILE* tga = NULL;
	vector *scanline;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {
		curscene = sceneLoad();
		tga = openTGA(curscene);
	} else
		curscene = calloc(1, sizeof(scene));

	MPI_Bcast((void*)curscene, sizeof(scene) / sizeof(float), MPI_FLOAT, 0, MPI_COMM_WORLD);

	res = sceneResolution(curscene);
	scanline = calloc(res, sizeof(vector));
	int line = 0;
	float aaweight = 1.0f / size;

	for (line = 0; line < res; line++) {
		if (rank == size - 1) {
			printf("Starting scan of horizontal line %d.\n", line);
		} else {
			MPI_Recv(scanline, (sizeof(vector) / sizeof(float)) * res, MPI_FLOAT, rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		}

		for (i = 0; i <res; i++)
		{
			if (rank == size - 1)
				scanline[i] = smul(pixelraytraceaa(curscene, line, i, rank), aaweight);
			else
				scanline[i] = add(scanline[i], smul(pixelraytraceaa(curscene, line, i, rank), aaweight));
		}

		if (rank == 0) {
			for (i = 0; i < res; i++)
				writeTGAColor(tga, curscene, line, i, scanline[i].x, scanline[i].y, scanline[i].z);
		} else {
			MPI_Send(scanline, (sizeof(vector) / sizeof(float)) * res, MPI_FLOAT, rank - 1, 0, MPI_COMM_WORLD);
		}
	}

	free(curscene);

	MPI_Finalize();

	return 0;
}

#endif
