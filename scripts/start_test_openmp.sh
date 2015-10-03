echo -e "Starting tests ....\n"
echo -e "Matrix 250x250 openmp"
../bin/openmp ../matrices/matriz250.txt ../output/openmp/matriz250
echo -e "Matrix 500x500 openmp"
../bin/openmp ../matrices/matriz500.txt  ../output/openmp/matriz500
echo -e "Matrix 1000x1000 openmp"
../bin/openmp ../matrices/matriz1000.txt  ../output/openmp/matriz1000
echo -e "Matrix 2000x2000 openmp"
../bin/openmp ../matrices/matriz2000.txt  ../output/openmp/matriz2000
echo -e "Matrix 4000x4000 openmp"
../bin/openmp ../matrices/matriz4000.txt  ../output/openmp/matriz4000

