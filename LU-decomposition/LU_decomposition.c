#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <cilk/cilk.h>

using namespace std;

void output(double[][100], int);
double a[100][100], l[100][100], u[100][100];
int n = 0, i = 0, j = 0, k = 0;

// Lower Decomposition Thread
void * Lower_thread(void* dummy)
{
    for (i = 0; i < n; i++)
    {
     	for (j = 0; j < n; j++)
        {
            if (j < i)
                l[j][i] = 0;
            else
            {
             	l[j][i] = a[j][i];
                for (k = 0; k < i; k++)
                {
                    l[j][i] = l[j][i] - l[j][k] * u[k][i];
                }
            }
	}
    }
    pthread_exit(NULL);
}
// Upper Decomposition Thread
void * Upper_thread(void* dummy)
{
    for (i = 0; i < n; i++)
    {

     	for (j = 0; j < n; j++)
        {
            if (j < i)
                  u[i][j] = 0;
            else if (j == i)
                u[i][j] = 1;
            else
            {
             	u[i][j] = a[i][j] / l[i][i];
                for (k = 0; k < i; k++)
{
                    l[j][i] = l[j][i] - l[j][k] * u[k][i];
                }
            }
	}
    }
    pthread_exit(NULL);
}
// Upper Decomposition Thread
void * Upper_thread(void* dummy)
{
    for (i = 0; i < n; i++)
    {

     	for (j = 0; j < n; j++)
        {
            if (j < i)
                  u[i][j] = 0;
            else if (j == i)
                u[i][j] = 1;
            else
            {
             	u[i][j] = a[i][j] / l[i][i];
                for (k = 0; k < i; k++)
                {
                    u[i][j] = u[i][j] - ((l[i][k] * u[k][j]) / l[i][i]);
                }
            }
	}
    }
    pthread_exit(NULL);
}
// Output Function
void output(double x[][100], int n)
{
    for (i = 0; i < n; i++)
    {
     	for (j = 0; j < n; j++)
        {
            std::cout << left << setw(5) << setprecision(3) << x[i][j] ;
        }
	cout << "\n";
    }
}
int main()
{
    cout << "Enter size of Square matrix : ";
    cin >> n;

