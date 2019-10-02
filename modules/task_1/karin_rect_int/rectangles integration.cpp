#include <mpi.h>
#include <iostream>
#include <cmath>
#include "rectangles integration.h"


double fun(double x) {
	return pow((x * 3), 3);
}

double Integration(double a, double b, int k) {
	double res = 0;
	double h = (b - a) / k;
	for (double x = a + h; x < b + 1e-6; x += h)
	{
		res += fun(x) * h;
	}
	return res;
}

double ParIntegration(double a, double b, int k){
	int rank;
	int size;

	double res,
		len,
		local_res,
		local_k,
		local_a,
		local_b;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	len = (b - a) / size;
	local_k = k / size;
	local_a = a + rank * len;
	local_b = local_a + len;

	local_res = Integration(local_a, local_b, local_k);

	MPI_Reduce(&local_res, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	return res;
}