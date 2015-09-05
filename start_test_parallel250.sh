echo -e "Starting tests ....\n"
echo -e "Matrix 250x250 3 threads"
./parallel matrices/matriz250.txt output/parallel/matrix250/output250_3 3
echo -e "\nMatrix 250x250 5 threads"
./parallel matrices/matriz250.txt output/parallel/matrix250/output250_5 5
echo -e "\nMatrix 250x250 7 threads"
./parallel matrices/matriz250.txt output/parallel/matrix250/output250_7 7 
echo -e "\nMatrix 250x250 10 threads"
./parallel matrices/matriz250.txt output/parallel/matrix250/output250_10 10
