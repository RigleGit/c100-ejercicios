cmake_minimum_required(VERSION 3.10)
project(puntero_a_funcion)

set(CMAKE_C_STANDARD 99)

# Crear el ejecutable principal
add_executable(puntero_a_funcion src/main.c)

# Incluir el directorio include si existe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(puntero_a_funcion PRIVATE include)
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
    add_executable(test_puntero_a_funcion test/test_main.c src/main.c)
    target_link_libraries(test_puntero_a_funcion ${CRITERION_LIBRARIES})
    target_include_directories(test_puntero_a_funcion PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_puntero_a_funcion PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_puntero_a_funcion PRIVATE ${CRITERION_CFLAGS_OTHER})
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_puntero_a_funcion PRIVATE include)
    endif()
    
    add_test(NAME puntero_a_funcion_tests COMMAND test_puntero_a_funcion)
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(WARNING "Para instalar Criterion en macOS: brew install criterion")
endif()

# Compilación con warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(puntero_a_funcion PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_puntero_a_funcion)
        target_compile_options(test_puntero_a_funcion PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
