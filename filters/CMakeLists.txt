cmake_minimum_required(VERSION 3.10)

project(Filters)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SourceFiles 
    src/helloworld.cpp
    src/seed.cpp
    src/findHits.cpp
    src/gapped.cpp
    src/anchor.cpp
    src/hspthres.cpp
    src/gapfree.cpp
    )

add_library(${PROJECT_NAME} SHARED ${SourceFiles})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE ../hits_stuff)
