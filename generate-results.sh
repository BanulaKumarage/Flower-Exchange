#!/bin/bash

# Compile your C++ program
g++ Exchange.cpp -o Exchange

# Run your program with generated input
for input_file in inputs/*.csv; do
    ./Exchange <<EOF
$input_file
EOF
done