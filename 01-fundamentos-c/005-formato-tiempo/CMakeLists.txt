cmake_minimum_required(VERSION 3.16)

# Configuración del proyecto
project(005-formato-tiempo LANGUAGES C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuración de flags de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

# Incluir directorios
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Definir ejecutable principal
add_executable(formato_tiempo src/main.c)

# Configuración para testing
enable_testing()

# Agregar test
add_executable(test_formato_tiempo test/test_main.c)
add_test(NAME FormatoTiempoTest COMMAND test_formato_tiempo)

# Configuración adicional para debugging
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(formato_tiempo PRIVATE -g -O0)
    target_compile_options(test_formato_tiempo PRIVATE -g -O0)
endif()
