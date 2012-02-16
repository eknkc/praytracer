all: raytracer

raytracer: staticmapping dynamicmapping sequential pipelinemapping

pipelinemapping: tracer mapping/pipelinemapping.c
	mpicc -c -O3 -DPIPELINEMAPPING mapping/pipelinemapping.c
	mpicc pipelinemapping.o functions.o scene.o tga.o tracer.o -o rtpline

sequential: tracer mapping/sequential.c
	mpicc -c -O3 -DSEQUENTIAL mapping/sequential.c
	mpicc sequential.o functions.o scene.o tga.o tracer.o -o rtsequential

dynamicmapping: tracer mapping/dynamicmapping.c
	mpicc -c -O3 -DDYNAMICMAPPING mapping/dynamicmapping.c
	mpicc dynamicmapping.o functions.o scene.o tga.o tracer.o -o rtdynamic

staticmapping: tracer mapping/staticmapping.c
	mpicc -c -O3 -DSTATICMAPPING mapping/staticmapping.c
	mpicc staticmapping.o functions.o scene.o tga.o tracer.o -o rtstatic
	
tracer: tracer/functions.c tracer/tga.c tracer/scene.c tracer/tracer.c
	mpicc -c -O3 tracer/functions.c
	mpicc -c -O3 tracer/tga.c
	mpicc -c -O3 tracer/scene.c
	mpicc -c -O3 tracer/tracer.c
	
clean:
	@rm -rf *.o rtstatic rtdynamic rtsequential rtpline scene.tga
