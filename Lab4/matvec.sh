
+mpicc -o matsearch.exe matsearch.c -std=c99 -lm
+mpirun -np 4 ./matsearch.exe