#include "mpi.h"
#include "../tracer/tracer.h"
#include "stdlib.h"
#include "math.h"

//#define STATICMAPPING

#ifdef STATICMAPPING

int main(int argc, char *argv[])
{
	initTracer();

	int i, j, p;
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

	double start, end;
	start = MPI_Wtime();

	MPI_Bcast((void*)curscene, sizeof(scene) / sizeof(float), MPI_FLOAT, 0, MPI_COMM_WORLD);

	res = sceneResolution(curscene);
	scanline = calloc(res, sizeof(vector));
	int lines = 0;

	for (j = rank; j < res; j += size)
	{
		printf("Process %d scanning horizontal line %d.\n", rank, j);
		for (i = 0; i <res; i++)
		{
			scanline[i] = pixelraytrace(curscene, i, j);
		}

		if (rank == 0) {
			for (i = 0; i < res; i++)
				writeTGAColor(tga, curscene, i, j, scanline[i].x, scanline[i].y, scanline[i].z);

			lines++;

			for (p = 1; p < size; p++) {

				lines++;

				if (lines > res) break;

				MPI_Recv(scanline, (sizeof(vector) / sizeof(float)) * res, MPI_FLOAT, p, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

				for (i = 0; i < res; i++)
					writeTGAColor(tga, curscene, i, status.MPI_TAG, scanline[i].x, scanline[i].y, scanline[i].z);
			}
		} else
			MPI_Send(scanline, (sizeof(vector) / sizeof(float)) * res, MPI_FLOAT, 0, j, MPI_COMM_WORLD);
	}

	free(curscene);

	end = MPI_Wtime();

	if (rank == 0)
		printf("Entire process took %.4f seconds.\n", end - start);

	MPI_Finalize();

	return 0;
}

#endif
