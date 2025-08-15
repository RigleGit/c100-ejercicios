cmake_minimum_required(VERSION 3.10)
project(redimensionar_array_realloc)

set(CMAKE_C_STANDARD 99)

# Crear el ejecutable principal
add_executable(redimensionar_array_realloc src/main.c src/redimensionar_array.c)

# Incluir el directorio include si existe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(redimensionar_array_realloc PRIVATE include)
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
    add_executable(test_redimensionar_array_realloc test/test_redimensionar_array.c src/redimensionar_array.c)
    target_link_libraries(test_redimensionar_array_realloc ${CRITERION_LIBRARIES})
    target_include_directories(test_redimensionar_array_realloc PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_redimensionar_array_realloc PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_redimensionar_array_realloc PRIVATE ${CRITERION_CFLAGS_OTHER})
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_redimensionar_array_realloc PRIVATE include)
    endif()
    
    add_test(NAME redimensionar_array_realloc_tests COMMAND test_redimensionar_array_realloc)
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(WARNING "Para instalar Criterion en macOS: brew install criterion")
endif()

# Compilación con warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(redimensionar_array_realloc PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_redimensionar_array_realloc)
        target_compile_options(test_redimensionar_array_realloc PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()