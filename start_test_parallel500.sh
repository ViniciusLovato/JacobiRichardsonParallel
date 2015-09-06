echo -e "Starting tests ....\n"
echo -e "Matrix 500x500 2 threads"
./parallel matrices/matriz500.txt output/parallel/matrix500/output500_2 2
echo -e "\nMatrix 500x500 4 threads"
./parallel matrices/matriz500.txt output/parallel/matrix500/output500_4 4
echo -e "\nMatrix 500x500 6 threads"
./parallel matrices/matriz500.txt output/parallel/matrix500/output500_6 6 
echo -e "\nMatrix 500x500 8 threads"
./parallel matrices/matriz500.txt output/parallel/matrix500/output500_8 8
echo -e "\nMatrix 500x500 10 threads"
./parallel matrices/matriz500.txt output/parallel/matrix500/output500_10 10
