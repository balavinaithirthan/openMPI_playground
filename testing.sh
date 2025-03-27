#!/bin/bash

# Define parameter values
declare -a np_values=(2 4 6 8 10 12 14 16)
declare -a filter_types=("compute_bound" "memory_bound")
declare -a filter_numbers=(5 10 20 25)
declare -a problem_sizes=(100 500 1000 5000 10000)

# Create an output log file
log_file="logs/all_runs.log"
mkdir -p logs
> "$log_file" # Clear previous log file

# Iterate over parameter combinations
for filter_type in "${filter_types[@]}"; do
#     for np in "${np_values[@]}"; do
#         echo "Running: mpirun -np $np ./MyMPIProject --filter_type=$filter_type --filter_number=10 --problem_size=10000" | tee -a "$log_file"
#         mpirun -np $np ./build/MyMPIProject --filter_type=$filter_type --filter_number=10 --problem_size=10000 &>> "$log_file"
#     done
#     for filter_number in "${filter_numbers[@]}"; do
#         echo "Running: mpirun -np 6 ./MyMPIProject --filter_type=$filter_type --filter_number=$filter_number --problem_size=10000" | tee -a "$log_file"
#         mpirun -np 6 ./build/MyMPIProject --filter_type=$filter_type --filter_number=$filter_number --problem_size=10000 &>> "$log_file"
#     done
#     for problem_size in "${problem_sizes[@]}"; do
#         echo "Running: mpirun -np 6 ./MyMPIProject --filter_type=$filter_type --filter_number=10 --problem_size=$problem_size" | tee -a "$log_file"
#         mpirun -np 2 * ./build/MyMPIProject --filter_type=$filter_type --filter_number=10 --problem_size=$(($problem_size * 2) &>> "$log_file"
#     done
# done
    echo "Running: mpirun -np 3 ./build/MyMPIProject --filter_type=$filter_type --filter_number=10 --problem_size=30000" | tee -a "$log_file"
    mpirun -np 3 ./build/MyMPIProject --filter_type=$filter_type --filter_number=10 --problem_size=30000 &>> "$log_file"
    echo "Running: mpirun -np 5 ./build/MyMPIProject --filter_type=$filter_type --filter_number=10 --problem_size=60000" | tee -a "$log_file"
    mpirun -np 5 ./build/MyMPIProject --filter_type=$filter_type --filter_number=10 --problem_size=60000 &>> "$log_file" 
    echo "Running: mpirun -np 9 ./build/MyMPIProject --filter_type=$filter_type --filter_number=10 --problem_size=120000" | tee -a "$log_file"
    mpirun -np 9 ./build/MyMPIProject --filter_type=$filter_type --filter_number=10 --problem_size=120000 &>> "$log_file" 
    echo "Running: mpirun -np 17 ./build/MyMPIProject --filter_type=$filter_type --filter_number=10 --problem_size=240000" | tee -a "$log_file"
    mpirun -np 17 ./build/MyMPIProject --filter_type=$filter_type --filter_number=10 --problem_size=240000 &>> "$log_file" 

done
echo "All simulations completed. Check logs/all_runs.log for outputs."
