cmake_minimum_required(VERSION 3.16)

# Configuración del proyecto
project(007-distancia-euclidiana LANGUAGES C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuración de flags de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

# Incluir directorios
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Definir ejecutable principal
add_executable(distancia_euclidiana src/main.c)

# Enlazar con la librería matemática
target_link_libraries(distancia_euclidiana m)

# Configuración para testing
enable_testing()

# Agregar test
add_executable(test_distancia_euclidiana test/test_main.c)
target_link_libraries(test_distancia_euclidiana m)
add_test(NAME DistanciaEuclidianaTest COMMAND test_distancia_euclidiana)

# Configuración adicional para debugging
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(distancia_euclidiana PRIVATE -g -O0)
    target_compile_options(test_distancia_euclidiana PRIVATE -g -O0)
endif()
