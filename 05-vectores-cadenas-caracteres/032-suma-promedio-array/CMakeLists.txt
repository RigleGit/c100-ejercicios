cmake_minimum_required(VERSION 3.10)
project(suma_promedio_array)

set(CMAKE_C_STANDARD 99)

# Crear el ejecutable principal
add_executable(suma_promedio_array src/main.c)

# Incluir el directorio include si existe
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(suma_promedio_array PRIVATE include)
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
    add_executable(test_suma_promedio_array test/test_main.c)
    target_link_libraries(test_suma_promedio_array ${CRITERION_LIBRARIES})
    target_include_directories(test_suma_promedio_array PRIVATE ${CRITERION_INCLUDE_DIRS})
    target_link_directories(test_suma_promedio_array PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_options(test_suma_promedio_array PRIVATE ${CRITERION_CFLAGS_OTHER})
    target_compile_definitions(test_suma_promedio_array PRIVATE UNIT_TESTING)
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_include_directories(test_suma_promedio_array PRIVATE include)
    endif()
    
    add_test(NAME suma_promedio_array_tests COMMAND test_suma_promedio_array)
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(WARNING "Para instalar Criterion en macOS: brew install criterion")
endif()

# Compilación con warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(suma_promedio_array PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET test_suma_promedio_array)
        target_compile_options(test_suma_promedio_array PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()