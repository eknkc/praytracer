#include "mpi.h"
#include "../tracer/tracer.h"
#include "stdlib.h"
#include "math.h"

#ifdef DYNAMICMAPPING

int main(int argc, char *argv[])
{
	initTracer();

	int i, p;
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

	if (rank == 0) {
		int lastline = 0;
		int waiting = res;

		for (lastline = 0; lastline < size - 1; lastline++) {
			MPI_Send(&lastline, 1, MPI_INT, lastline + 1, 0, MPI_COMM_WORLD);
		}

		int flag;

		while(1) {
			if (waiting == 0)
				break;

			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
			if (flag) {
				MPI_Recv(scanline, (sizeof(vector) / sizeof(float)) * res, MPI_FLOAT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);

				if (lastline < res) {
					MPI_Send(&lastline, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
					lastline++;
				}

				for (i = 0; i < res; i++)
					writeTGAColor(tga, curscene, status.MPI_TAG, i, scanline[i].x, scanline[i].y, scanline[i].z);

				waiting--;
			}
		}

		flag = -1;
		for (p = 1; p < size; p++) {
			MPI_Send(&flag, 1, MPI_INT, p, 0, MPI_COMM_WORLD);
		}

	} else {
		int line;

		while(1) {
			MPI_Recv(&line, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

			if (line < 0) {
				break;
			}

			printf("Process %d scanning horizontal line %d.\n", rank, line);

			for (i = 0; i < res; i++)
				scanline[i] = pixelraytrace(curscene, line, i);

			MPI_Send(scanline, (sizeof(vector) / sizeof(float)) * res, MPI_FLOAT, 0, line, MPI_COMM_WORLD);
		}
	}

	free(curscene);

	end = MPI_Wtime();

	if (rank == 0)
		printf("Entire process took %.4f seconds.\n", end - start);

	MPI_Finalize();

	return 0;
}

#endif
