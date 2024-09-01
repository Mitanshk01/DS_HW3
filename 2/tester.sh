#!/bin/bash

# Function to normalize spaces in a file
normalize_spaces() {
    sed -e 's/[[:space:]]\+/ /g' -e 's/[[:space:]]*$//' "$1" > "$2"
}

# Compile the test case generator and the brute-force script
g++ -o test_gen test_gen.cpp
g++ -o brute_force brute_force.cpp

# Compile the MPI program (assumed to be named mpi_julia.cpp)
mpic++ -o mpi_julia 2.cpp  

# Generate test cases
./test_gen

# Run the MPI program and brute-force program, then compare their outputs
mkdir -p results

for i in {1..15}; do
    echo "Running test case $i..."
    test_file="test_cases/test$i.txt"
    
    # Run brute-force method
    ./brute_force $test_file > results/brute_force_$i.txt

    # Run MPI method with 1 to 12 processes
    for np in {1..12}; do
        mpiexec -np $np --use-hwthread-cpus --oversubscribe ./mpi_julia < $test_file > results/mpi_$np_$i.txt

        # Normalize spaces in both outputs
        normalize_spaces "results/brute_force_$i.txt" "results/brute_force_normalized_$i.txt"
        normalize_spaces "results/mpi_$np_$i.txt" "results/mpi_normalized_$np_$i.txt"

        # Compare the normalized results
        if diff -q "results/brute_force_normalized_$i.txt" "results/mpi_normalized_$np_$i.txt" > /dev/null; then
            echo -e "Test case $i with $np processes: \e[32mPASSED\e[0m"
        else
            echo -e "Test case $i with $np processes: \e[31mFAILED\e[0m"
        fi
    done
done

# Clean up: Remove executables
rm -rf test_gen brute_force mpi_julia results/ test_cases/
