cmake_minimum_required(VERSION 3.10)
project(MyMPIProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find the MPI package
find_package(MPI REQUIRED)

# Add the executable target
add_executable(my_mpi_executable main.cpp)


# Link MPI to the target
target_link_libraries(my_mpi_executable PUBLIC MPI::MPI_CXX)
