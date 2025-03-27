
cmake_minimum_required(VERSION 3.10)
project(MyMPIProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Find the MPI package
find_package(MPI REQUIRED)

add_subdirectory(hits_lib)
add_subdirectory(load_balance)
add_subdirectory(filters)

set(CMAKE_BUILD_TYPE Release)


add_executable(${PROJECT_NAME} custom_pipeline.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC MPI::MPI_CXX)
target_link_libraries(${PROJECT_NAME} PUBLIC Load_Balance)
target_link_libraries(${PROJECT_NAME} PUBLIC Filters)
target_link_libraries(${PROJECT_NAME} PUBLIC hits_lib)

# target_include_directories(${PROJECT_NAME} PUBLIC load_balance)
# target_include_directories(${PROJECT_NAME} PUBLIC filters)
# target_include_directories(${PROJECT_NAME} PUBLIC hits_lib)
