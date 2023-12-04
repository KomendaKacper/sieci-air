cmake_minimum_required(VERSION 3.26)
project(Serwery-air)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        include
)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        )

add_executable(Serwery-air ${SOURCE_FILES} main.cpp include)


target_compile_definitions(Serwery-air PUBLIC EXERCISE_ID=<ID_ćwiczenia>)
