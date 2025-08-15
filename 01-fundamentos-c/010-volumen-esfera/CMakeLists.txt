cmake_minimum_required(VERSION 3.16)

# Configuración del proyecto
project(010-volumen-esfera LANGUAGES C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuración de flags de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

# Incluir directorios
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Definir ejecutable principal
add_executable(volumen_esfera src/main.c)

# Enlazar con la librería matemática
target_link_libraries(volumen_esfera m)

# Configuración para testing
enable_testing()

# Agregar test
add_executable(test_volumen_esfera test/test_main.c)
target_link_libraries(test_volumen_esfera m)
add_test(NAME VolumenEsferaTest COMMAND test_volumen_esfera)

# Configuración adicional para debugging
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(volumen_esfera PRIVATE -g -O0)
    target_compile_options(test_volumen_esfera PRIVATE -g -O0)
endif()

# Configuración para optimización en Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(volumen_esfera PRIVATE -O3)
    target_compile_options(test_volumen_esfera PRIVATE -O3)
endif()
