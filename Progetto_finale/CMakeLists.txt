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
