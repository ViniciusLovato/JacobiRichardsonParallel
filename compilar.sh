gcc main.c -o main -lm
gcc parallel.c -o parallel -lpthread -lm
gcc openmp.c -o openmp -fopenmp -lm
