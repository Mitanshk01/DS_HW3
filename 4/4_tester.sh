#!/bin/bash

# Compile the test case generator and the brute-force script
g++ -o test_gen 4_test_gen.cpp
g++ -o brute_force 4_brute_force.cpp

# Compile the MPI program
mpic++ -o mpi_matrix_inverse 4.cpp  

# Generate test cases
./test_gen

# Run the MPI program and brute-force program, then compare their outputs
mkdir -p results

normalize_spaces() {
    sed -e 's/[[:space:]]\+/ /g' -e 's/^[[:space:]]*//g' -e 's/[[:space:]]*$//g'
}

for i in {1..15}; do
    echo "Running test case $i..."
    test_file="test_cases/test$i.txt"
    
    # Run brute-force method
    ./brute_force $test_file | normalize_spaces > results/brute_force_$i.txt

    # Run MPI method with 1 to 12 processes
    for np in {1..12}; do
        mpiexec -np $np --use-hwthread-cpus --oversubscribe ./mpi_matrix_inversec $test_file | normalize_spaces > results/mpi_$np_$i.txt

        # Compare the results
        if diff -q results/brute_force_$i.txt results/mpi_$np_$i.txt > /dev/null; then
            echo -e "Test case $i with $np processes: \e[32mPASSED\e[0m"
        else
            echo -e "Test case $i with $np processes: \e[31mFAILED\e[0m"
        fi
    done
done

# Clean up: Remove executables
rm -rf test_gen brute_force mpi_matrix_inverse results/ test_cases/
