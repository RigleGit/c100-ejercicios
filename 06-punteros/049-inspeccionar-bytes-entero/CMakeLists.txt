cmake_minimum_required(VERSION 3.10)
project(inspeccionar_bytes_entero)

set(CMAKE_C_STANDARD 99)

# Crear el ejecutable principal
add_executable(inspeccionar_bytes_entero src/main.c)

# Incluir el directorio include si existe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(inspeccionar_bytes_entero PRIVATE include)
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
    add_executable(test_inspeccionar_bytes_entero test/test_main.c src/main.c)
    target_link_libraries(test_inspeccionar_bytes_entero ${CRITERION_LIBRARIES})
    target_include_directories(test_inspeccionar_bytes_entero PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_inspeccionar_bytes_entero PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_inspeccionar_bytes_entero PRIVATE ${CRITERION_CFLAGS_OTHER})
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_inspeccionar_bytes_entero PRIVATE include)
    endif()
    
    add_test(NAME inspeccionar_bytes_entero_tests COMMAND test_inspeccionar_bytes_entero)
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(WARNING "Para instalar Criterion en macOS: brew install criterion")
endif()

# Compilación con warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(inspeccionar_bytes_entero PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_inspeccionar_bytes_entero)
        target_compile_options(test_inspeccionar_bytes_entero PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
