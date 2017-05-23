#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
	int n;
	double *A, *Ap, *At;
	float value;
	int i, j, k,iter, p;
	int factorial;
	double sum, norm;

	FILE *filePointer;
	char inputFileName[250];
	char outputFileName[250];

	printf("Input file name: ");
	scanf("%s", inputFileName);
	printf("Output file name: ");
	scanf("%s", outputFileName);
	printf("p: ");
	scanf("%d", &p);

	filePointer = fopen(inputFileName, "r");
	if (!filePointer)
	{
		printf("Cannot open the file:\n %s\n", inputFileName);
		return 1;
	}

	/*Read the matrix A*/
	fscanf(filePointer, "%d", &n);

	A  = calloc(n*n, sizeof(double));
	Ap = calloc(n*n, sizeof(double));
	At = calloc(n*n, sizeof(double));

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			fscanf(filePointer, "%e", &value);
			A[i*n + j]  = value;
			Ap[i*n + j] = value;
		}

	fclose(filePointer);

	filePointer = fopen(outputFileName, "w");
	if (!filePointer)
	{
		printf("Cannot open the file:\n %s\n", inputFileName);
		return 1;
	}

	for (iter = 0; iter < p; iter++)
	{
		sum = 0;
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
			{
				sum += Ap[i*n + j]* Ap[i*n + j];
			}

		norm = sqrt(sum);

		factorial = 1;
		for (i = 0; i < iter+1; i++)
			factorial *= (i + 1);

		printf("%d  %f\n", iter+1,norm / factorial);
		fprintf(filePointer,"%d  %f\n", iter + 1, norm / factorial);

		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
			{
				sum = 0;
				for (k = 0; k < n; k++)
					sum += Ap[i*n + k] * A[k*n + j];
				At[i*n + j] = sum;
			}

		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				Ap[i*n + j] = At[i*n + j];
	}

	fclose(filePointer);

	return 0;
}


Madhuri Nannaware
Madhuri Nannaware
