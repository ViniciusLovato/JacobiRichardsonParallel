echo -e "Starting tests ....\n"
echo -e "Matrix 250x250 2 threads"
../bin/parallel ../matrices/matriz250.txt ../output/parallel/matrix250/output250_2 2
echo -e "\nMatrix 250x250 4 threads"
../bin/parallel ../matrices/matriz250.txt ../output/parallel/matrix250/output250_4 4
echo -e "\nMatrix 250x250 6 threads"
../bin/parallel ../matrices/matriz250.txt ../output/parallel/matrix250/output250_6 6 
echo -e "\nMatrix 250x250 8 threads"
../bin/parallel ../matrices/matriz250.txt ../output/parallel/matrix250/output250_8 8
echo -e "\nMatrix 250x250 10 threads"
../bin/parallel ../matrices/matriz250.txt ../output/parallel/matrix250/output250_10 10
