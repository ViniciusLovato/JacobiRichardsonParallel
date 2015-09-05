echo -e "Starting tests ....\n"
echo -e "\nMatrix 1000x1000 3 threads"
./parallel matrices/matriz1000.txt output/parallel/matrix1000/output1000_3 3
echo -e "\nMatrix 1000x1000 5 threads"
./parallel matrices/matriz1000.txt output/parallel/matrix1000/output1000_5 5
echo -e "\nMatrix 1000x1000 7 threads"
./parallel matrices/matriz1000.txt output/parallel/matrix1000/output1000_7 7 
echo -e "\nMatrix 1000x1000 10 threads"
./parallel matrices/matriz1000.txt output/parallel/matrix1000/output1000_10 10
