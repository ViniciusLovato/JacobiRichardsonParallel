echo -e "Starting tests ....\n"
echo -e "Matrix 1000x1000 2 threads"
./parallel matrices/matriz1000.txt output/parallel/matrix1000/output1000_2 2
echo -e "\nMatrix 1000x1000 4 threads"
./parallel matrices/matriz1000.txt output/parallel/matrix1000/output1000_4 4
echo -e "\nMatrix 1000x1000 6 threads"
./parallel matrices/matriz1000.txt output/parallel/matrix1000/output1000_6 6 
echo -e "\nMatrix 1000x1000 8 threads"
./parallel matrices/matriz1000.txt output/parallel/matrix1000/output1000_8 8
echo -e "\nMatrix 1000x1000 10 threads"
./parallel matrices/matriz1000.txt output/parallel/matrix1000/output1000_10 10
