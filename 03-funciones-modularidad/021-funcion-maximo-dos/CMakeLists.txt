cmake_minimum_required(VERSION 3.10)
project(funcion_maximo_dos)

set(CMAKE_C_STANDARD 99)

# Crear el ejecutable principal
add_executable(funcion_maximo_dos src/main.c)

# Incluir el directorio include si existe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(funcion_maximo_dos PRIVATE include)
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
    add_executable(test_funcion_maximo_dos test/test_main.c)
    target_link_libraries(test_funcion_maximo_dos ${CRITERION_LIBRARIES})
    target_include_directories(test_funcion_maximo_dos PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_funcion_maximo_dos PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_funcion_maximo_dos PRIVATE ${CRITERION_CFLAGS_OTHER})
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_funcion_maximo_dos PRIVATE include)
    endif()
    
    add_test(NAME funcion_maximo_dos_tests COMMAND test_funcion_maximo_dos)
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(WARNING "Para instalar Criterion en macOS: brew install criterion")
endif()

# Compilación con warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(funcion_maximo_dos PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_funcion_maximo_dos)
        target_compile_options(test_funcion_maximo_dos PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
