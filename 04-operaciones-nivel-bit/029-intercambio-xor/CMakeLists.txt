cmake_minimum_required(VERSION 3.10)
project(InterambioXOR)

# Configurar estándar de C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configurar flags de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

# Crear ejecutable principal
add_executable(intercambio_xor src/main.c)

# Configurar directorio de inclusión
target_include_directories(intercambio_xor PRIVATE include)

# Habilitar testing
enable_testing()

# Crear ejecutable de pruebas
add_executable(test_intercambio_xor test/test_main.c)
target_include_directories(test_intercambio_xor PRIVATE include)

# Agregar prueba
add_test(
    NAME test_intercambio_xor
    COMMAND test_intercambio_xor
)
