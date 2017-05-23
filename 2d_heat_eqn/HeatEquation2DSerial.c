#include <stdio.h>
//#include <conio.h>
#include <stdlib.h> 
#include <math.h> 
#include <time.h>


int main(int argc, char **argv)
{
	int Ndt, dt;                         /*Ndt is number of steps of time to be carried out */
	                                     /* dt is time intervaD */
	double L, h, x, y, k, alpha, beta;      /*all i/p parameters */
	int N, n, i, j;                      /* N=n^2 and is for original grid */
	double *a, *aNew, *aTemp;
	FILE *outputFile;
	char outputFileName[] = "OutputFile.txt";
	char outputFileNameStart[] = "OutputFileStart.txt";
	clock_t begin, end;
	double time_spent;

	L = 1; /* dimmension of the problem */
	k = 0.00001; /* dt */

	if (argc != 3) {
		fprintf(stderr, "Usage: n Ndt\n");
		exit(0);
	}

	
	n = atoi(argv[1]);                  /* Reading square grid (row, coumn) dimension and number of time steps from command Line i.e. n & Ndt*/
	Ndt = atoi(argv[2]);

	h		= L / (n - 1);                   /*input formulas as given */
	alpha	= k / (h*h);
	beta	= 1 - 4 * alpha;

	printf("\nn                = %d\n", n);
	printf("Ndt= %d\n", Ndt);

	N = n*n; //size of matrix n x n

	a = (double *)malloc(N * sizeof(double));             /*space allocation */
	aNew = (double *)malloc(N * sizeof(double));
	
	for (i = 0; i < n; i++) {                              /* define A*/
		x = i*h;
		for (j = 0; j < n; j++)
		{
			y = j*h;
			a[i * n + j] = 100 * x*y*(L - x)*(L - y);
			aNew[i * n + j] = a[i * n + j];
		}
	}

	outputFile = fopen(outputFileNameStart, "w");
	if (outputFile == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			fprintf(outputFile, "%10.6f ", a[i * n + j]);
		fprintf(outputFile, "\n");
	}
	fprintf(outputFile, "\n");
	fclose(outputFile);

	begin = clock();

	dt = 0;
	while (dt < Ndt)
	{
		for (i = 1; i < n-1; i++)
			for (j = 1; j < n-1; j++)
			{
				aNew[i*n+j] = beta*a[i*n + j] + alpha*(a[(i - 1)*n + j] + a[(i + 1)*n + j] + a[i*n + j - 1] + a[i*n + j + 1]);  /*5 point avg updation*/
			}

		dt++;

		aTemp = a;                     /*update for next iteration */
		a = aNew;
		aNew = aTemp;
	}

	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;                  /*print time */

	printf("\nTime: %f\n", time_spent);

	outputFile = fopen(outputFileName, "w");
	if (outputFile == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			fprintf(outputFile, "%10.6f ", a[i * n + j]);
		fprintf(outputFile, "\n");
	}
	fprintf(outputFile, "\n");

	fclose(outputFile);

	return 0;
}
