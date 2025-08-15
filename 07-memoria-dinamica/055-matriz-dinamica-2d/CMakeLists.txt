cmake_minimum_required(VERSION 3.10)
project(matriz_dinamica_2d)

set(CMAKE_C_STANDARD 99)

# Crear el ejecutable principal
add_executable(matriz_dinamica_2d src/main.c src/matriz_dinamica_2d.c)

# Incluir el directorio include si existe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(matriz_dinamica_2d PRIVATE include)
endif()

# Configurar las pruebas
enable_testing()

# Buscar Criterion (framework de testing para C)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    # Si Criterion está disponible, compilar y ejecutar las pruebas
    add_executable(test_matriz_dinamica_2d tests/test_matriz_dinamica_2d.c src/matriz_dinamica_2d.c)
    target_link_libraries(test_matriz_dinamica_2d ${CRITERION_LIBRARIES})
    target_include_directories(test_matriz_dinamica_2d PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_matriz_dinamica_2d PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_matriz_dinamica_2d PRIVATE ${CRITERION_CFLAGS_OTHER})
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_matriz_dinamica_2d PRIVATE include)
    endif()
    
    add_test(NAME matriz_dinamica_2d_tests COMMAND test_matriz_dinamica_2d)
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(WARNING "Para instalar Criterion en macOS: brew install criterion")
endif()

# Compilación con warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(matriz_dinamica_2d PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_matriz_dinamica_2d)
        target_compile_options(test_matriz_dinamica_2d PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()