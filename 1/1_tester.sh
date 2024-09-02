#!/bin/bash

# Compile the test case generator and the brute-force script
g++ -o test_gen_knn 1_test_gen.cpp
g++ -o bruteforce_knn 1_brute_force.cpp

# Compile the MPI program
mpic++ -o mpi_knn 1.cpp  # Replace '3_knn.cpp' with your actual MPI program filename

# Generate test cases
./test_gen_knn

# Create a function to preprocess files to normalize spaces
normalize_spaces() {
    sed -e 's/[[:space:]]\+/ /g' -e 's/[[:space:]]*$//' "$1" > "$2"
}

# Run the MPI program and brute-force program, then compare their outputs
mkdir -p results

for i in {1..15}; do
    echo "Running test case $i..."
    test_file="test_cases/test$i.txt"
    
    # Run brute-force method
    ./bruteforce_knn $test_file > results/bruteforce_knn_$i.txt

    # Run MPI method with 1 to 12 processes
    for np in {1..12}; do
        mpiexec -np $np --use-hwthread-cpus --oversubscribe ./mpi_knn $test_file > results/mpi_knn_$np_$i.txt

        # Normalize spaces in output files
        normalize_spaces results/bruteforce_knn_$i.txt results/bruteforce_knn_$i_normalized.txt
        normalize_spaces results/mpi_knn_$np_$i.txt results/mpi_knn_$np_$i_normalized.txt

        # Compare the results
        if diff -q results/bruteforce_knn_$i_normalized.txt results/mpi_knn_$np_$i_normalized.txt > /dev/null; then
            echo -e "Test case $i with $np processes: \e[32mPASSED\e[0m"
        else
            echo -e "Test case $i with $np processes: \e[31mFAILED\e[0m"
        fi
    done
done

# Clean up: Remove executables and directories
rm -rf test_gen_knn bruteforce_knn mpi_knn results/ test_cases/
