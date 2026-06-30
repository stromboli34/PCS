cmake_minimum_required(VERSION 3.20)
project(final_project CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



find_package(Eigen3 3.3 REQUIRED NO_MODULE)


add_executable(circuit main.cpp)
target_link_libraries(circuit PRIVATE Eigen3::Eigen)



add_executable(benchmark_cicli benchmark_cicli.cpp)
target_link_libraries(benchmark_cicli PRIVATE Eigen3::Eigen)
target_include_directories(benchmark_cicli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(experiment_cond experiment_cond.cpp)
target_link_libraries(experiment_cond PRIVATE Eigen3::Eigen)
target_include_directories(experiment_cond PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

