#!/bin/bash

# Compile the MPI program
mpic++ -o mpi_program 5.cpp

# File to store the output
output_file="time_output.txt"

# Input file  
input_file="input.txt"

# Empty the output file if it already exists
> $output_file

# Run the MPI program with 1 to 12 processes, passing input from input.txt
for n in {1..12}; do
    echo "Running with $n processes:" >> $output_file
    mpiexec -np $n --use-hwthread-cpus --oversubscribe ./mpi_program < $input_file >> $output_file
    echo "" >> $output_file
done

echo "Execution completed. Results are stored in $output_file"
