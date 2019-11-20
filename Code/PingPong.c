#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[]) {
	int data, source, destination, np;
	int count = 1;
	double start, end, counter;
	FILE *fp;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &source);

	// modify "data.txt" file to change to the desired integer message.
     	fp = fopen("data.txt", "r");
     	fscanf(fp, "%d", &data);
	fclose(fp);

	if (source%2 == 1) {
		destination = source - 1;
	}
	else {
		destination = source + 1;
	}
	if (source%2 == 0) {
		start = MPI_Wtime();
		MPI_Send(&data, count, MPI_INT, destination, source, MPI_COMM_WORLD);
		MPI_Recv(&data, count, MPI_INT, destination, destination, MPI_COMM_WORLD, &status);
		end = MPI_Wtime();
		printf("Sending time for message 1: %.6fs\n", end - start);

		counter = end - start;
		MPI_Send(&counter, count, MPI_DOUBLE, destination, 123, MPI_COMM_WORLD);
	} else {
		start = MPI_Wtime();
		MPI_Recv(&data, count, MPI_INT, destination, destination, MPI_COMM_WORLD, &status);
		MPI_Send(&data, count, MPI_INT, destination, source, MPI_COMM_WORLD);
		end = MPI_Wtime();
		printf("Sending time for message 2: %.6fs\n", end - start);

		double inputCounter;
		MPI_Recv(&inputCounter, count, MPI_DOUBLE, destination, 123, MPI_COMM_WORLD, &status);
		counter = (end - start) + inputCounter;
		printf("Ping-Pong time: %.6fs\n", counter);
	}
	printf("Sent %d to proc %d, received %d from proc %d\n", data, destination, data, destination);
	MPI_Finalize();
	return 0;
}
