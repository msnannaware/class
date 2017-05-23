#include <stdio.h>
#include <stdlib.h> 
#include <math.h> 
#include <mpi.h> 
#include <string.h>



int main(int argc, char **argv)
{
	int Ndt, dt;                                       /*Ndt is number of steps of time to be carried out */
	                                                   /* dt is time intervaD */
	double L, h, x, y, k, alpha, beta;                 /*all i/p parameters */
	int   numtasks, taskid;
	int N, n, i, j, ia, ja, nforDB, NforDB;             /* N=n^2 and is for original grid  , NforDB=nforDB^2 and is for distributed blocks */
	double *a, *ABlock, *ABlockNew, *ALocalTemp;        /*variables for local distributed blocks created*/
	double sum;
	double start_time, end_time;
	MPI_Datatype blockTypeGlobal, blockTypeLocal, rowType, columnType;
	int *disps, dims[2], periods[2];
	int proc, proc1, proc2, proc3;
	MPI_Comm comm2d;
	int my2drank;
	int numberOfBlocks, rowsize;
	int MyCoords[2], coords[2];
	MPI_Status status;
	int row = 0, col = 1;
	FILE *outputFile;
	char outFileName[] = "OutputFile.txt";
	char outputFileTime[] = "OutputFileTime.txt";

	L = 1; 
	k = 0.00001; 

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

	if (argc < 3) {
		if (taskid == 0) {
			fprintf(stderr, "Usage: n Ndt\n");
		}
		MPI_Finalize();
		exit(0);
	}

	n = atoi(argv[1]);               /* Reading square grid dimension and number of time steps from command Line i.e. n & Ndt*/
	Ndt = atoi(argv[2]);

/*input formulas as given */
	h = L / (n - 1);
	alpha = k / (h*h);
	beta = 1 - 4 * alpha;

	if (taskid == 0) {
		printf("\nn                = %d\n", n);
		printf("Ndt= %d\n", Ndt);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	dims[row] = dims[col] = (int)sqrt(numtasks);  /*no. of rows & Columns equal to tasks to be performed in each iteration i.e. n */

	periods[row] = periods[col] = 0;             /*setting false i.e. grid is non periodic in both row & column */

	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &comm2d);
	MPI_Comm_rank(comm2d, &my2drank);
	MPI_Cart_coords(comm2d, my2drank, 2, MyCoords);

	N = n*n; //size of matrix n x n
	numberOfBlocks = (int)sqrt(numtasks);
	nforDB = n / numberOfBlocks;
	NforDB = nforDB*nforDB;
	rowsize = numberOfBlocks*NforDB;

	if (my2drank == 0) {
		a = (double *)malloc(N * sizeof(double));

		disps = (int *)malloc(numtasks * sizeof(int));

		for (i = 0; i < n; i++) {
			x = i*h;
			for (j = 0; j < n; j++)
			{
				y = j*h;
				a[i * n + j] = 100 * x*y*(L - x)*(L - y);
			}
		}

		for (i = 0; i < numberOfBlocks; i++)
			for (j = 0; j < numberOfBlocks; j++) {
				coords[row] = i;
				coords[col] = j;
				MPI_Cart_rank(comm2d, coords, &proc);
				disps[proc] = i*rowsize + j*nforDB;
			}
	}

	ABlock = (double *)calloc((nforDB + 2)*(nforDB + 2), sizeof(double));                        /* space allocation for grid and block storage */
	ABlockNew = (double *)calloc((nforDB + 2)*(nforDB + 2), sizeof(double));

	MPI_Type_vector(nforDB, nforDB, n, MPI_DOUBLE, &blockTypeGlobal);
	MPI_Type_commit(&blockTypeGlobal);

	MPI_Type_vector(nforDB, nforDB, nforDB + 2, MPI_DOUBLE, &blockTypeLocal);
	MPI_Type_commit(&blockTypeLocal);

	MPI_Type_contiguous(nforDB, MPI_DOUBLE, &rowType);
	MPI_Type_commit(&rowType);

	MPI_Type_vector(nforDB, 1, (nforDB + 2), MPI_DOUBLE, &columnType);
	MPI_Type_commit(&columnType);

	/* block division into right, left, top, bottom corners & middle / centre and their respective calculations and message passing for next iterations */
	
	if (my2drank == 0) {
		for (i = 0; i < nforDB; i++)
			for (j = 0; j < nforDB; j++)
			{
				ABlock[(i + 1)*(nforDB + 2) + j + 1] = a[i*n + j];
				ABlockNew[(i + 1)*(nforDB + 2) + j + 1] = ABlock[(i + 1)*(nforDB + 2) + j + 1];
			}

		for (i = 1; i < numtasks; i++)
			MPI_Send(a + disps[i], 1, blockTypeGlobal, i, 1, comm2d);
	}

	if (my2drank != 0) {
		MPI_Recv(ABlock + nforDB + 3, 1, blockTypeLocal, 0, 1, comm2d, &status);
		for (i = 0; i < nforDB; i++)
			for (j = 0; j < nforDB; j++)
				ABlockNew[(i + 1)*(nforDB + 2) + j + 1] = ABlock[(i + 1)*(nforDB + 2) + j + 1];
	}

	MPI_Barrier(comm2d);

	if (my2drank == 0)
		start_time = MPI_Wtime();

	dt = 0;
	while (dt < Ndt)
	{
		if ((MyCoords[0] == 0) && (MyCoords[1] == 0))
		{
			coords[row] = MyCoords[row] + 1;      /*data exchanges from bottom corners */ 
			coords[col] = MyCoords[col];
			MPI_Cart_rank(comm2d, coords, &proc);
			MPI_Send(ABlock + (nforDB)*(nforDB + 2) + 1, 1, rowType, proc, 1, comm2d);

			MPI_Recv(ABlock + (nforDB + 1)*(nforDB + 2) + 1, 1, rowType, proc, 1, comm2d, &status);

			
			coords[row] = MyCoords[row];                  /* Right  side corners*/
			coords[col] = MyCoords[col] + 1;
			MPI_Cart_rank(comm2d, coords, &proc2);

			MPI_Send(ABlock + (nforDB + 2) + nforDB + 0, 1, columnType, proc2, 0, comm2d);
			MPI_Recv(ABlock + (nforDB + 2) + nforDB + 1, 1, columnType, proc2, 0, comm2d, &status);

			for (i = 1; i < nforDB; i++)                                  /* calculating local new values & storing inputs/updates for next iteration */
				for (j = 1; j < nforDB; j++)
				{
					sum = beta*ABlock[(i + 1)*(nforDB + 2) + j + 1];
					ia = i - 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i + 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j - 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j + 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];

					ABlockNew[(i + 1)*(nforDB + 2) + j + 1] = sum;
				}
		}
		else if ((MyCoords[0] == 0) && (MyCoords[1] == numberOfBlocks - 1))
		{
			coords[row] = MyCoords[row] + 1;
			coords[col] = MyCoords[col];
			MPI_Cart_rank(comm2d, coords, &proc);

			MPI_Send(ABlock + (nforDB)*(nforDB + 2) + 1, 1, rowType, proc, 1, comm2d);
			MPI_Recv(ABlock + (nforDB + 1)*(nforDB + 2) + 1, 1, rowType, proc, 1, comm2d, &status);

			coords[row] = MyCoords[row];
			coords[col] = MyCoords[col] - 1;
			MPI_Cart_rank(comm2d, coords, &proc3);

			MPI_Send(ABlock + (nforDB + 2) + 1, 1, columnType, proc3, 0, comm2d);
			MPI_Recv(ABlock + (nforDB + 2) + 0, 1, columnType, proc3, 0, comm2d, &status);

			for (i = 1; i < nforDB; i++)
				for (j = 0; j < nforDB - 1; j++)
				{
					sum = beta*ABlock[(i + 1)*(nforDB + 2) + j + 1];
					ia = i - 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i + 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j - 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j + 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];

					ABlockNew[(i + 1)*(nforDB + 2) + j + 1] = sum;
				}

		}
		else if ((MyCoords[0] == numberOfBlocks - 1) && (MyCoords[1] == 0))
		{
			coords[row] = MyCoords[row] - 1;
			coords[col] = MyCoords[col];
			MPI_Cart_rank(comm2d, coords, &proc);

			MPI_Send(ABlock + (nforDB + 2) + 1, 1, rowType, proc, 1, comm2d);
			MPI_Recv(ABlock + 1, 1, rowType, proc, 1, comm2d, &status);

			coords[row] = MyCoords[row];
			coords[col] = MyCoords[col] + 1;
			MPI_Cart_rank(comm2d, coords, &proc2);

			MPI_Send(ABlock + (nforDB + 2) + nforDB + 0, 1, columnType, proc2, 0, comm2d);
			MPI_Recv(ABlock + (nforDB + 2) + nforDB + 1, 1, columnType, proc2, 0, comm2d, &status);

			for (i = 0; i < nforDB - 1; i++)
				for (j = 1; j < nforDB; j++)
				{
					sum = beta*ABlock[(i + 1)*(nforDB + 2) + j + 1];
					ia = i - 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i + 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j - 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j + 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];

					ABlockNew[(i + 1)*(nforDB + 2) + j + 1] = sum;
				}
		}
		else if ((MyCoords[0] == numberOfBlocks - 1) && (MyCoords[1] == numberOfBlocks - 1))
		{
			coords[row] = MyCoords[row] - 1;
			coords[col] = MyCoords[col];
			MPI_Cart_rank(comm2d, coords, &proc);
			MPI_Recv(ABlock + 1, 1, rowType, proc, 1, comm2d, &status);
			MPI_Send(ABlock + (nforDB + 2) + 1, 1, rowType, proc, 1, comm2d);

			/* Left */
			coords[row] = MyCoords[row];
			coords[col] = MyCoords[col] - 1;
			MPI_Cart_rank(comm2d, coords, &proc3);

			MPI_Send(ABlock + (nforDB + 2) + 1, 1, columnType, proc3, 0, comm2d);
			MPI_Recv(ABlock + (nforDB + 2) + 0, 1, columnType, proc3, 0, comm2d, &status);

			for (i = 0; i < nforDB - 1; i++)
				for (j = 0; j < nforDB - 1; j++)
				{
					sum = beta*ABlock[(i + 1)*(nforDB + 2) + j + 1];
					ia = i - 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i + 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j - 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j + 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];

					ABlockNew[(i + 1)*(nforDB + 2) + j + 1] = sum;
				}
		}
		else if ((MyCoords[0] == 0) && (MyCoords[1] > 0))
		{
			coords[row] = MyCoords[row] + 1;
			coords[col] = MyCoords[col];
			MPI_Cart_rank(comm2d, coords, &proc);
			MPI_Send(ABlock + (nforDB)*(nforDB + 2) + 1, 1, rowType, proc, 1, comm2d);
			MPI_Recv(ABlock + (nforDB + 1)*(nforDB + 2) + 1, 1, rowType, proc, 1, comm2d, &status);

			coords[row] = MyCoords[row];
			coords[col] = MyCoords[col] - 1;
			MPI_Cart_rank(comm2d, coords, &proc3);

			MPI_Send(ABlock + (nforDB + 2) + 1, 1, columnType, proc3, 0, comm2d);
			MPI_Recv(ABlock + (nforDB + 2) + 0, 1, columnType, proc3, 0, comm2d, &status);

			coords[row] = MyCoords[row];
			coords[col] = MyCoords[col] + 1;
			MPI_Cart_rank(comm2d, coords, &proc2);

			MPI_Send(ABlock + (nforDB + 2) + nforDB + 0, 1, columnType, proc2, 0, comm2d);
			MPI_Recv(ABlock + (nforDB + 2) + nforDB + 1, 1, columnType, proc2, 0, comm2d, &status);

			for (i = 1; i < nforDB; i++)
				for (j = 0; j < nforDB; j++)
				{
					sum = beta*ABlock[(i + 1)*(nforDB + 2) + j + 1];
					ia = i - 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i + 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j - 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j + 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];

					ABlockNew[(i + 1)*(nforDB + 2) + j + 1] = sum;
				}
		}
		else if ((MyCoords[0] == numberOfBlocks - 1) && (MyCoords[1] > 0))
		{
			coords[row] = MyCoords[row] - 1;
			coords[col] = MyCoords[col];
			MPI_Cart_rank(comm2d, coords, &proc);
			MPI_Recv(ABlock + 1, 1, rowType, proc, 1, comm2d, &status);
			MPI_Send(ABlock + (nforDB + 2) + 1, 1, rowType, proc, 1, comm2d);

			/* Left (n,1) */
			coords[row] = MyCoords[row];
			coords[col] = MyCoords[col] - 1;
			MPI_Cart_rank(comm2d, coords, &proc3);

			MPI_Send(ABlock + (nforDB + 2) + 1, 1, columnType, proc3, 0, comm2d);
			MPI_Recv(ABlock + (nforDB + 2) + 0, 1, columnType, proc3, 0, comm2d, &status);

			/*(1,n) right */
			coords[row] = MyCoords[row];
			coords[col] = MyCoords[col] + 1;
			MPI_Cart_rank(comm2d, coords, &proc2);

			MPI_Send(ABlock + (nforDB + 2) + nforDB + 0, 1, columnType, proc2, 0, comm2d);
			MPI_Recv(ABlock + (nforDB + 2) + nforDB + 1, 1, columnType, proc2, 0, comm2d, &status);

			for (i = 0; i < nforDB - 1; i++)
				for (j = 0; j < nforDB; j++)
				{
					sum = beta*ABlock[(i + 1)*(nforDB + 2) + j + 1];
					ia = i - 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i + 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j - 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j + 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];

					ABlockNew[(i + 1)*(nforDB + 2) + j + 1] = sum;
				}
		}
		else if ((MyCoords[0] > 0) && (MyCoords[1] == 0))
		{
			/* bottom leftmost */
			coords[row] = MyCoords[row] - 1;
			coords[col] = MyCoords[col];
			MPI_Cart_rank(comm2d, coords, &proc);

			MPI_Send(ABlock + (nforDB + 2) + 1, 1, rowType, proc, 1, comm2d);
			MPI_Recv(ABlock + 1, 1, rowType, proc, 1, comm2d, &status);

			/*topmost left*/
			coords[row] = MyCoords[row] + 1;
			coords[col] = MyCoords[col];
			MPI_Cart_rank(comm2d, coords, &proc1);

			MPI_Send(ABlock + nforDB*(nforDB + 2) + 1, 1, rowType, proc1, 1, comm2d);
			MPI_Recv(ABlock + (nforDB + 1)*(nforDB + 2) + 1, 1, rowType, proc1, 1, comm2d, &status);

			/* Right */
			coords[row] = MyCoords[row];
			coords[col] = MyCoords[col] + 1;
			MPI_Cart_rank(comm2d, coords, &proc2);

			MPI_Send(ABlock + (nforDB + 2) + nforDB + 0, 1, columnType, proc2, 0, comm2d);
			MPI_Recv(ABlock + (nforDB + 2) + nforDB + 1, 1, columnType, proc2, 0, comm2d, &status);

			for (i = 0; i < nforDB; i++)
				for (j = 1; j < nforDB; j++)
				{
					sum = beta*ABlock[(i + 1)*(nforDB + 2) + j + 1];
					ia = i - 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i + 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j - 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j + 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];

					ABlockNew[(i + 1)*(nforDB + 2) + j + 1] = sum;
				}
		}
		else if ((MyCoords[0] > 0) && (MyCoords[1] == numberOfBlocks - 1))
		{
			coords[row] = MyCoords[row] - 1;
			coords[col] = MyCoords[col];
			MPI_Cart_rank(comm2d, coords, &proc);

			MPI_Send(ABlock + (nforDB + 2) + 1, 1, rowType, proc, 1, comm2d);
			MPI_Recv(ABlock + 1, 1, rowType, proc, 1, comm2d, &status);

			coords[row] = MyCoords[row] + 1;
			coords[col] = MyCoords[col];
			MPI_Cart_rank(comm2d, coords, &proc1);

			MPI_Send(ABlock + nforDB*(nforDB + 2) + 1, 1, rowType, proc1, 1, comm2d);
			MPI_Recv(ABlock + (nforDB + 1)*(nforDB + 2) + 1, 1, rowType, proc1, 1, comm2d, &status);

			coords[row] = MyCoords[row];
			coords[col] = MyCoords[col] - 1;
			MPI_Cart_rank(comm2d, coords, &proc3);

			MPI_Send(ABlock + (nforDB + 2) + 1, 1, columnType, proc3, 0, comm2d);
			MPI_Recv(ABlock + (nforDB + 2) + 0, 1, columnType, proc3, 0, comm2d, &status);

			for (i = 0; i < nforDB; i++)
				for (j = 0; j < nforDB - 1; j++)
				{
					sum = beta*ABlock[(i + 1)*(nforDB + 2) + j + 1];
					ia = i - 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i + 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j - 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j + 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];

					ABlockNew[(i + 1)*(nforDB + 2) + j + 1] = sum;
				}
		}
		else
		{
			/*  rightmost bottom */
			coords[row] = MyCoords[row] - 1;
			coords[col] = MyCoords[col];
			MPI_Cart_rank(comm2d, coords, &proc);

			MPI_Send(ABlock + (nforDB + 2) + 1, 1, rowType, proc, 1, comm2d);
			MPI_Recv(ABlock + 1, 1, rowType, proc, 1, comm2d, &status);

			/* rightmost top */
			coords[row] = MyCoords[row] + 1;
			coords[col] = MyCoords[col];
			MPI_Cart_rank(comm2d, coords, &proc1);

			MPI_Send(ABlock + nforDB*(nforDB + 2) + 1, 1, rowType, proc1, 1, comm2d);
			MPI_Recv(ABlock + (nforDB + 1)*(nforDB + 2) + 1, 1, rowType, proc1, 1, comm2d, &status);

			coords[row] = MyCoords[row];
			coords[col] = MyCoords[col] - 1;
			MPI_Cart_rank(comm2d, coords, &proc3);

			MPI_Send(ABlock + (nforDB + 2) + 1, 1, columnType, proc3, 0, comm2d);
			MPI_Recv(ABlock + (nforDB + 2) + 0, 1, columnType, proc3, 0, comm2d, &status);

			coords[row] = MyCoords[row];
			coords[col] = MyCoords[col] + 1;
			MPI_Cart_rank(comm2d, coords, &proc2);

			MPI_Send(ABlock + (nforDB + 2) + nforDB + 0, 1, columnType, proc2, 0, comm2d);
			MPI_Recv(ABlock + (nforDB + 2) + nforDB + 1, 1, columnType, proc2, 0, comm2d, &status);

			/* updating inputs for next iteration */ 
			for (i = 0; i < nforDB; i++)
				for (j = 0; j < nforDB; j++)
				{
					sum = beta*ABlock[(i + 1)*(nforDB + 2) + j + 1];
					ia = i - 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i + 1; ja = j;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j - 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];
					ia = i; ja = j + 1;
					sum += alpha*ABlock[(ia + 1)*(nforDB + 2) + ja + 1];

					ABlockNew[(i + 1)*(nforDB + 2) + j + 1] = sum;
				}
		}

		MPI_Barrier(comm2d);

		ALocalTemp = ABlock;
		ABlock = ABlockNew;
		ABlockNew = ALocalTemp;

		dt++;
	}

	if (my2drank != 0) {
		MPI_Send(ABlock + nforDB + 3, 1, blockTypeLocal, 0, 1, comm2d);
	}

	if (my2drank == 0) {
		for (i = 0; i < nforDB; i++)
			for (j = 0; j < nforDB; j++)
				a[i*n + j] = ABlock[(i + 1)*(nforDB + 2) + j + 1];

		for (i = 1; i < numtasks; i++)
			MPI_Recv(a + disps[i], 1, blockTypeGlobal, i, 1, comm2d, &status);
	}

	MPI_Barrier(comm2d);

	if (my2drank == 0)
		end_time = MPI_Wtime();

	if (my2drank == 0)
		printf("Wallclock time: %f sec\n", end_time - start_time);

	if (my2drank == 0)
	{
		outputFile = fopen(outFileName, "w");
		if (outputFile == NULL)
		{
			MPI_Finalize();
			exit(1);
		}

		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++)
				fprintf(outputFile, "%10.6f ", a[i * n + j]);
			fprintf(outputFile, "\n");
		}
		fprintf(outputFile, "\n");

		fclose(outputFile);

		outputFile = fopen(outputFileTime, "w");
		if (outputFile == NULL)
		{
			exit(1);
		}

		fprintf(outputFile, "Time: %f", (double)(end_time - start_time));

		fprintf(outputFile, "\n");
		fclose(outputFile);
	}

	if (my2drank == 0)
		printf("Stop\n");

	MPI_Finalize();

	return 0;
}