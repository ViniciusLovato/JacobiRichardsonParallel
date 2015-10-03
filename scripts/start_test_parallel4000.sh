echo -e "Starting tests ....\n"
echo -e "Matrix 4000x4000 2 threads"
../bin/parallel ../matrices/matriz4000.txt ../output/parallel/matrix4000/output4000_2 2
echo -e "\nMatrix 4000x4000 4 threads"
../bin/parallel ../matrices/matriz4000.txt ../output/parallel/matrix4000/output4000_4 4
echo -e "\nMatrix 4000x4000 6 threads"
../bin/parallel ../matrices/matriz4000.txt ../output/parallel/matrix4000/output4000_6 6 
echo -e "\nMatrix 4000x4000 8 threads"
../bin/parallel ../matrices/matriz4000.txt ../output/parallel/matrix4000/output4000_8 8
echo -e "\nMatrix 4000x4000 10 threads"
../bin/parallel ../matrices/matriz4000.txt ../output/parallel/matrix4000/output4000_10 10
