cmake_minimum_required(VERSION 3.10)

project(Load_Balance)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SourceFiles 
    src/helloworld.cpp
    src/load_balance_indices.cpp
    src/children.cpp
    src/parent.cpp
    src/filters_load_balance.cpp
    src/mpi_kernel.cpp
    src/timing.cpp
    )

add_library(${PROJECT_NAME} SHARED ${SourceFiles})

target_include_directories(Load_Balance PUBLIC include) #Include directory for the Load_Balance library, offer it to its dependencies
target_include_directories(Load_Balance PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) #Include directory for the Load_Balance library, offer it to its dependencies
target_link_libraries(Load_Balance PUBLIC hits_lib) #Link to the hits_lib library
target_link_libraries(Load_Balance PUBLIC MPI::MPI_CXX)

# get_target_property(LINK_LIBS Load_Balance LINK_LIBRARIES)
# message(STATUS "MyTarget links to: ${LINK_LIBS}")


