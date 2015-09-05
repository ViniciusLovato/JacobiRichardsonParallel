echo -e "Starting tests ....\n"
echo -e "\nMatrix 2000x2000 3 threads"
./parallel matrices/matriz2000.txt output/parallel/matrix2000/output2000_3 3
echo -e "\nMatrix 2000x2000 5 threads"
./parallel matrices/matriz2000.txt output/parallel/matrix2000/output2000_5 5
echo -e "\nMatrix 2000x2000 7 threads"
./parallel matrices/matriz2000.txt output/parallel/matrix2000/output2000_7 7 
echo -e "\nMatrix 2000x2000 10 threads"
./parallel matrices/matriz2000.txt output/parallel/matrix2000/output2000_10 10
