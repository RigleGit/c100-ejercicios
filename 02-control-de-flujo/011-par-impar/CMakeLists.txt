cmake_minimum_required(VERSION 3.16)

# Configuración del proyecto
project(011-par-impar LANGUAGES C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuración de flags de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

# Incluir directorios
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Definir ejecutable principal
add_executable(par_impar src/main.c)

# Configuración para testing
enable_testing()

# Agregar test
add_executable(test_par_impar test/test_main.c)
add_test(NAME ParImparTest COMMAND test_par_impar)

# Configuración adicional para debugging
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(par_impar PRIVATE -g -O0)
    target_compile_options(test_par_impar PRIVATE -g -O0)
endif()

# Configuración para optimización en Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(par_impar PRIVATE -O3)
    target_compile_options(test_par_impar PRIVATE -O3)
endif()
