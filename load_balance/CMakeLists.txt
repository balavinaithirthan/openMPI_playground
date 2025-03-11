cmake_minimum_required(VERSION 3.10)

project(Load_Balance)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SourceFiles 
    src/helloworld.cpp
    src/debug.cpp
    src/load_balance_indices.cpp
    src/children.cpp
    src/parent.cpp
    src/filters_load_balance.cpp
    src/mpi_kernel.cpp
    src/timing.cpp
    )

add_library(${PROJECT_NAME} SHARED ${SourceFiles})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE ../hits_stuff) # TODO: pull out the Hit.hpp file and put it in a separate include directory
target_link_libraries(${PROJECT_NAME} PUBLIC MPI::MPI_CXX)

