cmake_minimum_required(VERSION 3.20)
project(final_project CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Flag di warning: GCC/Clang vs MSVC
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wshadow -Wextra)
endif()

# Eigen è header-only: target imported Eigen3::Eigen disponibile dopo find_package
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# eseguibile principale
add_executable(circuit main.cpp)
target_link_libraries(circuit PRIVATE Eigen3::Eigen)

# include path della directory del progetto, per gli header locali (.hpp)
target_include_directories(circuit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# --- suite di test ---
enable_testing()

# 1) mattoni di base: un_edge, un_graph, selectionSort
add_executable(test_bricks test_bricks.cpp)
target_link_libraries(test_bricks PRIVATE Eigen3::Eigen)
target_include_directories(test_bricks PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME test_bricks COMMAND test_bricks)

# 2) lettura della netlist
add_executable(test_read test_read.cpp)
target_link_libraries(test_read PRIVATE Eigen3::Eigen)
target_include_directories(test_read PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME test_read
         COMMAND test_read
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# 3) soluzione del circuito e proprieta' dei cicli
add_executable(test_solver test_solver.cpp)
target_link_libraries(test_solver PRIVATE Eigen3::Eigen)
target_include_directories(test_solver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME test_solver
         COMMAND test_solver
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
		 
# --- esperimenti (sempre ottimizzati -O2) ---
add_executable(benchmark_cicli benchmark_cicli.cpp)
target_link_libraries(benchmark_cicli PRIVATE Eigen3::Eigen)
target_include_directories(benchmark_cicli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(experiment_cond experiment_cond.cpp)
target_link_libraries(experiment_cond PRIVATE Eigen3::Eigen)
target_include_directories(experiment_cond PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT MSVC)
    target_compile_options(benchmark_cicli PRIVATE -O2)
    target_compile_options(experiment_cond PRIVATE -O2)
endif()
