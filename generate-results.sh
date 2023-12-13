#!/bin/bash

# Compile your C++ program
g++ Exchange.cpp -o Exchange

# Run your program with generated input
for input_file in inputs/*.csv; do
    echo "Generating the execution report for order - $input_file"
    start_time=$(date +%s.%N)
    ./Exchange <<EOF
$input_file
EOF
    end_time=$(date +%s.%N)
    execution_time=$(echo "$end_time - $start_time" | bc)
    echo "Execution time for $input_file: $execution_time seconds"
    echo "-------------------------------------------------------"
done