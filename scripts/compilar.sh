gcc ../src/main.c -o ../bin/main -lm
gcc ../src/parallel.c -o ../bin/parallel -lpthread -lm
gcc ../src/openmp.c -o ../bin/openmp -fopenmp -lm
