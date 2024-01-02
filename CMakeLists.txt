cmake_minimum_required(VERSION 3.26)
project(Serwery-air)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        include
)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        src/nodes.cpp
        src/helpers.cpp
        src/factory.cpp
        include/types.hpp
        include/storage_types.hpp
        include/nodes.hpp
        include/helpers.hpp
        include/factory.hpp
        googletest-master
        )

add_executable(Serwery-air ${SOURCE_FILES} main.cpp)

# Ustaw zmienną `SOURCES_FILES_TESTS`, która będzie przechowywać ścieżki do
# plików źródłowych z testami jednostkowymi.
set(SOURCES_FILES_TESTS
        test/main_gtest.cpp
        test/test_Factory.cpp
        test/test_factory_io.cpp
        test/test_nodes.cpp
        test/test_package.cpp
        test/test_reports.cpp
        test/test_simulate.cpp
        test/test_storage_types.cpp
        )

# Dodaj konfigurację typu `Test`.
add_executable(NetSim_test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

# Dodaj podkatalog `googletest-master`.
add_subdirectory(googletest-master)

# Dodaj katalogi z plikami nagłówkowymi wyłącznie do konkretnej konfiguracji (tu: `Test`).
target_include_directories(NetSim_test PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
        )

target_compile_definitions(Serwery-air PUBLIC EXERCISE_ID=<ID_ćwiczenia>)
