cmake_minimum_required(VERSION 3.10)
project(array_dinamico_cadenas)

set(CMAKE_C_STANDARD 99)

# Crear el ejecutable principal
add_executable(array_dinamico_cadenas src/main.c src/array_dinamico_cadenas.c)

# Incluir el directorio include si existe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(array_dinamico_cadenas PRIVATE include)
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
    add_executable(test_array_dinamico_cadenas tests/test_array_dinamico_cadenas.c src/array_dinamico_cadenas.c)
    target_link_libraries(test_array_dinamico_cadenas ${CRITERION_LIBRARIES})
    target_include_directories(test_array_dinamico_cadenas PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_array_dinamico_cadenas PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_array_dinamico_cadenas PRIVATE ${CRITERION_CFLAGS_OTHER})
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_array_dinamico_cadenas PRIVATE include)
    endif()
    
    add_test(NAME array_dinamico_cadenas_tests COMMAND test_array_dinamico_cadenas)
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(WARNING "Para instalar Criterion en macOS: brew install criterion")
endif()

# Compilación con warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(array_dinamico_cadenas PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_array_dinamico_cadenas)
        target_compile_options(test_array_dinamico_cadenas PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()