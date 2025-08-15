cmake_minimum_required(VERSION 3.10)
project(triangulo_asteriscos)

set(CMAKE_C_STANDARD 99)

# Crear el ejecutable principal
add_executable(triangulo_asteriscos src/main.c)

# Incluir el directorio include si existe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(triangulo_asteriscos PRIVATE include)
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
    add_executable(test_triangulo_asteriscos test/test_main.c)
    target_link_libraries(test_triangulo_asteriscos ${CRITERION_LIBRARIES})
    target_include_directories(test_triangulo_asteriscos PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_triangulo_asteriscos PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_triangulo_asteriscos PRIVATE ${CRITERION_CFLAGS_OTHER})
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_triangulo_asteriscos PRIVATE include)
    endif()
    
    add_test(NAME triangulo_asteriscos_tests COMMAND test_triangulo_asteriscos)
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(WARNING "Para instalar Criterion en macOS: brew install criterion")
endif()

# Compilación con warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(triangulo_asteriscos PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_triangulo_asteriscos)
        target_compile_options(test_triangulo_asteriscos PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
