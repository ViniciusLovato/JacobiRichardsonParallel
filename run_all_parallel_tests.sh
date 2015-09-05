echo "Running all parallel tests"
echo -e "\nRunning parallel 250x250"
./start_test_parallel250.sh
echo -e "\nRunning parallel 500x500"
./start_test_parallel500.sh
echo -e "\nRunning parallel 1000x1000"
./start_test_parallel1000.sh
echo -e "\nRunning parallel 2000x2000"
./start_test_parallel2000.sh
echo -e "\nRunning parallel 4000x4000"
./start_test_parallel4000.sh
