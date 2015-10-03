echo -e "Starting tests ....\n"
echo -e "Matrix 2000x2000 2 threads"
../bin/parallel ../matrices/matriz2000.txt ../output/parallel/matrix2000/output2000_2 2
echo -e "\nMatrix 2000x2000 4 threads"
../bin/parallel ../matrices/matriz2000.txt ../output/parallel/matrix2000/output2000_4 4
echo -e "\nMatrix 2000x2000 6 threads"
../bin/parallel ../matrices/matriz2000.txt ../output/parallel/matrix2000/output2000_6 6 
echo -e "\nMatrix 2000x2000 8 threads"
../bin/parallel ../matrices/matriz2000.txt ../output/parallel/matrix2000/output2000_8 8
echo -e "\nMatrix 2000x2000 10 threads"
../bin/parallel ../matrices/matriz2000.txt ../output/parallel/matrix2000/output2000_10 10
