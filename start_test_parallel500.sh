echo -e "Starting tests ....\n"
echo -e "\nMatrix 500x500 3 threads"
./parallel matrices/matriz500.txt output/parallel/matrix500/output500_3 3
echo -e "\nMatrix 500x500 5 threads"
./parallel matrices/matriz500.txt output/parallel/matrix500/output500_5 5
echo -e "\nMatrix 500x500 7 threads"
./parallel matrices/matriz500.txt output/parallel/matrix500/output500_7 7 
echo -e "\nMatrix 500x500 10 threads"
./parallel matrices/matriz500.txt output/parallel/matrix500/output500_10 10
