cmake_minimum_required(VERSION 3.10)
project(suma_matrices)

set(CMAKE_C_STANDARD 99)

# Crear el ejecutable principal
add_executable(suma_matrices src/main.c)

# Incluir el directorio include si existe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(suma_matrices PRIVATE include)
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
    add_executable(test_suma_matrices test/test_main.c)
    target_link_libraries(test_suma_matrices ${CRITERION_LIBRARIES})
    target_include_directories(test_suma_matrices PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_suma_matrices PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_suma_matrices PRIVATE ${CRITERION_CFLAGS_OTHER})
    target_compile_definitions(test_suma_matrices PRIVATE UNIT_TESTING)
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_suma_matrices PRIVATE include)
    endif()
    
    add_test(NAME suma_matrices_tests COMMAND test_suma_matrices)
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(WARNING "Para instalar Criterion en macOS: brew install criterion")
endif()

# Compilación con warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(suma_matrices PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_suma_matrices)
        target_compile_options(test_suma_matrices PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()