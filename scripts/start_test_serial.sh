echo -e "Starting tests ....\n"
echo -e "Matrix 250x250"
../bin/main ../matrices/matriz250.txt ../output/serial/output250 
echo -e "\nMatrix 500x500"
../bin/main ../matrices/matriz500.txt ../output/serial/output500 
echo -e "\nMatrix 1000x1000"
../bin/main ../matrices/matriz1000.txt ../output/serial/output1000 
echo -e "\nMatrix 2000x2000"
../bin/main ../matrices/matriz2000.txt ../output/serial/output2000 
echo -e "\nMatrix 4000x4000"
../bin/main ../matrices/matriz4000.txt ../output/serial/output4000
