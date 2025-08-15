cmake_minimum_required(VERSION 3.10)
project(invertir_cadena)

set(CMAKE_C_STANDARD 99)

# Crear el ejecutable principal
add_executable(invertir_cadena src/main.c)

# Incluir el directorio include si existe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(invertir_cadena PRIVATE include)
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
    add_executable(test_invertir_cadena test/test_main.c src/main.c)
    target_link_libraries(test_invertir_cadena ${CRITERION_LIBRARIES})
    target_include_directories(test_invertir_cadena PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_invertir_cadena PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_invertir_cadena PRIVATE ${CRITERION_CFLAGS_OTHER})
    target_compile_definitions(test_invertir_cadena PRIVATE UNIT_TESTING)
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_invertir_cadena PRIVATE include)
    endif()
    
    add_test(NAME invertir_cadena_tests COMMAND test_invertir_cadena)
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(WARNING "Para instalar Criterion en macOS: brew install criterion")
endif()

# Compilación con warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(invertir_cadena PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_invertir_cadena)
        target_compile_options(test_invertir_cadena PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()