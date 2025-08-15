cmake_minimum_required(VERSION 3.10)
project(constantes_preprocesador C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Opciones de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -DDEBUG=1")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -DNDEBUG")

# Directorios de inclusión
include_directories(include)

# ============================================================================
# BIBLIOTECAS
# ============================================================================

# Biblioteca principal
add_library(constantes_preprocesador STATIC
    src/constantes_preprocesador.c
)

target_include_directories(constantes_preprocesador PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================================================
# EJECUTABLES
# ============================================================================

# Ejecutable principal (función main_usuario_basico)
add_executable(main_basico
    src/constantes_preprocesador.c
)

target_compile_definitions(main_basico PRIVATE COMPILAR_MAIN_BASICO)
target_include_directories(main_basico PRIVATE include)

# Ejecutable de demostración completa
add_executable(demo_completo
    src/constantes_preprocesador.c
)

target_compile_definitions(demo_completo PRIVATE COMPILAR_DEMO_COMPLETO)
target_include_directories(demo_completo PRIVATE include)

# Ejecutable con información de debug
add_executable(demo_debug
    src/constantes_preprocesador.c
)

target_compile_definitions(demo_debug PRIVATE 
    COMPILAR_DEMO_COMPLETO
    DEBUG=1
    HABILITAR_LOGGING=1
)
target_include_directories(demo_debug PRIVATE include)

# ============================================================================
# TESTS CON CRITERION
# ============================================================================

# Buscar Criterion
find_library(CRITERION_LIB criterion REQUIRED)
find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
if(NOT CRITERION_LIB OR NOT CRITERION_INCLUDE_DIR)
    message(FATAL_ERROR "Criterion not found. Please install criterion (headers and library).")
endif()

# Ejecutable de tests
add_executable(test_constantes_preprocesador
    tests/test_constantes_preprocesador.c
    src/constantes_preprocesador.c
)

target_include_directories(test_constantes_preprocesador PRIVATE
    include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CRITERION_INCLUDE_DIR}
)

target_link_libraries(test_constantes_preprocesador 
    ${CRITERION_LIB}
    m  # libm para funciones matemáticas si son necesarias
)

# Habilitar testing
enable_testing()

# Agregar test
add_test(NAME test_constantes_preprocesador 
         COMMAND test_constantes_preprocesador)

# ============================================================================
# TARGETS PERSONALIZADOS
# ============================================================================

# Target para ejecutar tests con salida detallada
add_custom_target(test_verbose
    COMMAND ${CMAKE_BINARY_DIR}/test_constantes_preprocesador --verbose
    DEPENDS test_constantes_preprocesador
    COMMENT "Ejecutando tests con salida detallada"
)

# Target para ejecutar tests con criterio específico
add_custom_target(test_basicos
    COMMAND ${CMAKE_BINARY_DIR}/test_constantes_preprocesador --filter constantes_basicas*
    DEPENDS test_constantes_preprocesador
    COMMENT "Ejecutando solo tests básicos"
)

# Target para ejecutar tests de matemáticas
add_custom_target(test_matematicas
    COMMAND ${CMAKE_BINARY_DIR}/test_constantes_preprocesador --filter constantes_matematicas*
    DEPENDS test_constantes_preprocesador
    COMMENT "Ejecutando tests de constantes matemáticas"
)

# Target para ejecutar tests educativos
add_custom_target(test_educativos
    COMMAND ${CMAKE_BINARY_DIR}/test_constantes_preprocesador --filter educativo*
    DEPENDS test_constantes_preprocesador
    COMMENT "Ejecutando tests educativos"
)

# Target para demostrar diferencias en compilación
add_custom_target(demo_compilacion
    COMMAND echo "=== Compilación con DEBUG ===" && ${CMAKE_BINARY_DIR}/demo_debug
    COMMAND echo "=== Compilación SIN DEBUG ===" && ${CMAKE_BINARY_DIR}/demo_completo
    DEPENDS demo_debug demo_completo
    COMMENT "Demostrando diferencias en compilación con/sin DEBUG"
)

# Target para mostrar expansión de preprocesador
add_custom_target(mostrar_expansion
    COMMAND ${CMAKE_C_COMPILER} -E ${CMAKE_CURRENT_SOURCE_DIR}/src/constantes_preprocesador.c -I${CMAKE_CURRENT_SOURCE_DIR}/include -DCOMPILAR_DEMO_COMPLETO | head -100
    COMMENT "Mostrando expansión del preprocesador (primeras 100 líneas)"
)

# ============================================================================
# INSTALACIÓN
# ============================================================================

# Instalar biblioteca
install(TARGETS constantes_preprocesador
        ARCHIVE DESTINATION lib)

# Instalar headers
install(FILES include/constantes_preprocesador.h
        DESTINATION include)

# Instalar ejecutables
install(TARGETS main_basico demo_completo demo_debug
        RUNTIME DESTINATION bin)

# ============================================================================
# INFORMACIÓN DE BUILD
# ============================================================================

# Mostrar información de configuración
message(STATUS "========================================")
message(STATUS "Configuración del proyecto:")
message(STATUS "  Nombre: ${PROJECT_NAME}")
message(STATUS "  Estándar C: C${CMAKE_C_STANDARD}")
message(STATUS "  Tipo de build: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compilador: ${CMAKE_C_COMPILER}")
message(STATUS "  Flags C: ${CMAKE_C_FLAGS}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "  Flags Debug: ${CMAKE_C_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "  Flags Release: ${CMAKE_C_FLAGS_RELEASE}")
endif()
message(STATUS "  Criterion encontrado: ${CRITERION_LIB}")
message(STATUS "========================================")

# ============================================================================
# DOCUMENTACIÓN
# ============================================================================

# Target para generar documentación con Doxygen si está disponible
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
    set(DOXYGEN_PROJECT_NAME "Constantes de Preprocesador")
    set(DOXYGEN_PROJECT_BRIEF "Ejercicio educativo sobre #define en C")
    
    doxygen_add_docs(docs
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        COMMENT "Generando documentación con Doxygen"
    )
    
    message(STATUS "Doxygen encontrado - target 'docs' disponible")
else()
    message(STATUS "Doxygen no encontrado - documentación no disponible")
endif()
