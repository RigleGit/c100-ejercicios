cmake_minimum_required(VERSION 3.16)

# Configuración del proyecto
project(008-promedio-tres-numeros LANGUAGES C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuración de flags de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

# Incluir directorios
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Definir ejecutable principal
add_executable(promedio_tres_numeros src/main.c)

# Configuración para testing
enable_testing()

# Agregar test
add_executable(test_promedio_tres_numeros test/test_main.c)
add_test(NAME PromedioTresNumerosTest COMMAND test_promedio_tres_numeros)

# Configuración adicional para debugging
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(promedio_tres_numeros PRIVATE -g -O0)
    target_compile_options(test_promedio_tres_numeros PRIVATE -g -O0)
endif()
