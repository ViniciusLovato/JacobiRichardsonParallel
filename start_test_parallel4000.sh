echo -e "Starting tests ....\n"
echo -e "\nMatrix 4000x4000 3 threads"
./parallel matrices/matriz4000.txt output/parallel/matrix4000/output4000_3 3
echo -e "\nMatrix 4000x4000 5 threads"
./parallel matrices/matriz4000.txt output/parallel/matrix4000/output4000_5 5
echo -e "\nMatrix 4000x4000 7 threads"
./parallel matrices/matriz4000.txt output/parallel/matrix4000/output4000_7 7 
echo -e "\nMatrix 4000x4000 10 threads"
./parallel matrices/matriz4000.txt output/parallel/matrix4000/output4000_10 10
