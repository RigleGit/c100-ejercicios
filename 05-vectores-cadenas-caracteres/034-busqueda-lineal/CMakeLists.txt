cmake_minimum_required(VERSION 3.10)
project(busqueda_lineal)

set(CMAKE_C_STANDARD 99)

# Crear el ejecutable principal
add_executable(busqueda_lineal src/main.c)

# Incluir el directorio include si existe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(busqueda_lineal PRIVATE include)
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
    add_executable(test_busqueda_lineal test/test_main.c)
    target_link_libraries(test_busqueda_lineal ${CRITERION_LIBRARIES})
    target_include_directories(test_busqueda_lineal PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_busqueda_lineal PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_busqueda_lineal PRIVATE ${CRITERION_CFLAGS_OTHER})
    target_compile_definitions(test_busqueda_lineal PRIVATE UNIT_TESTING)
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_busqueda_lineal PRIVATE include)
    endif()
    
    add_test(NAME busqueda_lineal_tests COMMAND test_busqueda_lineal)
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(WARNING "Para instalar Criterion en macOS: brew install criterion")
endif()

# Compilación con warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(busqueda_lineal PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_busqueda_lineal)
        target_compile_options(test_busqueda_lineal PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()