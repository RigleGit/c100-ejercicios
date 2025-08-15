cmake_minimum_required(VERSION 3.10)
project(funcion_esprimo)

set(CMAKE_C_STANDARD 99)

# Crear el ejecutable principal
add_executable(funcion_esprimo src/main.c)

# Incluir el directorio include si existe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(funcion_esprimo PRIVATE include)
endif()

# Enlazar con la biblioteca matemática
target_link_libraries(funcion_esprimo m)

# Configurar las pruebas
enable_testing()

# Buscar Criterion (framework de testing para C)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

if(CRITERION_FOUND)
    # Si Criterion está disponible, compilar y ejecutar las pruebas
    add_executable(test_funcion_esprimo test/test_main.c)
    target_link_libraries(test_funcion_esprimo ${CRITERION_LIBRARIES} m)
    target_include_directories(test_funcion_esprimo PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_funcion_esprimo PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_funcion_esprimo PRIVATE ${CRITERION_CFLAGS_OTHER})
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_funcion_esprimo PRIVATE include)
    endif()
    
    add_test(NAME funcion_esprimo_tests COMMAND test_funcion_esprimo)
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(WARNING "Para instalar Criterion en macOS: brew install criterion")
endif()

# Compilación con warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(funcion_esprimo PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_funcion_esprimo)
        target_compile_options(test_funcion_esprimo PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
