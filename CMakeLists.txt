cmake_minimum_required(VERSION 3.10)
project(MyMPIProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Find the MPI package
find_package(MPI REQUIRED)

add_subdirectory(Load_Balance)

set(CMAKE_BUILD_TYPE Debug)


add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC MPI::MPI_CXX)
target_include_directories(${PROJECT_NAME} PRIVATE load_balance)
target_link_libraries(${PROJECT_NAME} PUBLIC Load_Balance)