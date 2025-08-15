cmake_minimum_required(VERSION 3.10)
project(compilacion_condicional C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Opciones de compilación base
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -DNDEBUG")

# Directorios de inclusión
include_directories(include)

# ============================================================================
# OPCIONES PERSONALIZADAS
# ============================================================================

option(ENABLE_DEBUG "Habilitar mensajes de debug" OFF)
option(ENABLE_DEBUG_VERBOSE "Habilitar debug verbose" OFF)
option(ENABLE_DEBUG_TRACE "Habilitar trazado de funciones" OFF)
option(ENABLE_DEBUG_MEMORY "Habilitar debug de memoria" OFF)
option(ENABLE_DEBUG_TIMING "Habilitar medición de tiempos" OFF)
option(ENABLE_FEATURE_LOGGING "Habilitar sistema de logging" OFF)
option(ENABLE_FEATURE_STATS "Habilitar estadísticas" OFF)
option(ENABLE_FEATURE_PROFILING "Habilitar profiling" OFF)

# Configurar definiciones basadas en opciones
if(ENABLE_DEBUG)
    add_definitions(-DDEBUG)
endif()

if(ENABLE_DEBUG_VERBOSE)
    add_definitions(-DDEBUG_VERBOSE)
endif()

if(ENABLE_DEBUG_TRACE)
    add_definitions(-DDEBUG_TRACE)
endif()

if(ENABLE_DEBUG_MEMORY)
    add_definitions(-DDEBUG_MEMORY)
endif()

if(ENABLE_DEBUG_TIMING)
    add_definitions(-DDEBUG_TIMING)
endif()

if(ENABLE_FEATURE_LOGGING)
    add_definitions(-DFEATURE_LOGGING)
endif()

if(ENABLE_FEATURE_STATS)
    add_definitions(-DFEATURE_STATS)
endif()

if(ENABLE_FEATURE_PROFILING)
    add_definitions(-DFEATURE_PROFILING)
endif()

# ============================================================================
# BIBLIOTECAS
# ============================================================================

# Biblioteca principal
add_library(compilacion_condicional STATIC
    src/compilacion_condicional.c
)

target_include_directories(compilacion_condicional PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================================================
# EJECUTABLES PRINCIPALES
# ============================================================================

# 1. Ejecutable básico del enunciado (SIN DEBUG)
add_executable(main_sin_debug
    src/compilacion_condicional.c
)
target_compile_definitions(main_sin_debug PRIVATE COMPILAR_MAIN_BASICO)
target_include_directories(main_sin_debug PRIVATE include)

# 2. Ejecutable básico del enunciado (CON DEBUG)
add_executable(main_con_debug
    src/compilacion_condicional.c
)
target_compile_definitions(main_con_debug PRIVATE 
    COMPILAR_MAIN_BASICO
    DEBUG
)
target_include_directories(main_con_debug PRIVATE include)

# 3. Demostración completa (configuración mínima)
add_executable(demo_minimo
    src/compilacion_condicional.c
)
target_compile_definitions(demo_minimo PRIVATE COMPILAR_DEMO_COMPLETO)
target_include_directories(demo_minimo PRIVATE include)

# 4. Demostración con DEBUG básico
add_executable(demo_debug_basico
    src/compilacion_condicional.c
)
target_compile_definitions(demo_debug_basico PRIVATE 
    COMPILAR_DEMO_COMPLETO
    DEBUG
)
target_include_directories(demo_debug_basico PRIVATE include)

# 5. Demostración con DEBUG completo
add_executable(demo_debug_completo
    src/compilacion_condicional.c
)
target_compile_definitions(demo_debug_completo PRIVATE 
    COMPILAR_DEMO_COMPLETO
    DEBUG
    DEBUG_VERBOSE
    DEBUG_TRACE
    DEBUG_MEMORY
    DEBUG_TIMING
)
target_include_directories(demo_debug_completo PRIVATE include)

# 6. Demostración con todas las características
add_executable(demo_todas_caracteristicas
    src/compilacion_condicional.c
)
target_compile_definitions(demo_todas_caracteristicas PRIVATE 
    COMPILAR_DEMO_COMPLETO
    DEBUG
    DEBUG_VERBOSE
    DEBUG_TRACE
    DEBUG_MEMORY
    DEBUG_TIMING
    FEATURE_LOGGING
    FEATURE_STATS
    FEATURE_PROFILING
)
target_include_directories(demo_todas_caracteristicas PRIVATE include)

# 7. Versión de producción (optimizada, sin debug)
add_executable(demo_produccion
    src/compilacion_condicional.c
)
target_compile_definitions(demo_produccion PRIVATE 
    COMPILAR_DEMO_COMPLETO
    NDEBUG
)
target_compile_options(demo_produccion PRIVATE -O2)
target_include_directories(demo_produccion PRIVATE include)

# ============================================================================
# TESTS CON CRITERION
# ============================================================================

# Buscar Criterion
find_library(CRITERION_LIB criterion REQUIRED)
find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
if(NOT CRITERION_LIB OR NOT CRITERION_INCLUDE_DIR)
    message(FATAL_ERROR "Criterion not found. Please install criterion (headers and library).")
endif()

# Tests sin DEBUG
add_executable(test_sin_debug
    tests/test_compilacion_condicional.c
    src/compilacion_condicional.c
)
target_include_directories(test_sin_debug PRIVATE include)
target_include_directories(test_sin_debug PRIVATE ${CRITERION_INCLUDE_DIR})
target_link_libraries(test_sin_debug ${CRITERION_LIB})

# Tests con DEBUG básico
add_executable(test_con_debug
    tests/test_compilacion_condicional.c
    src/compilacion_condicional.c
)
target_compile_definitions(test_con_debug PRIVATE DEBUG)
target_include_directories(test_con_debug PRIVATE include)
target_include_directories(test_con_debug PRIVATE ${CRITERION_INCLUDE_DIR})
target_link_libraries(test_con_debug ${CRITERION_LIB})

# Tests con DEBUG completo
add_executable(test_debug_completo
    tests/test_compilacion_condicional.c
    src/compilacion_condicional.c
)
target_compile_definitions(test_debug_completo PRIVATE 
    DEBUG
    DEBUG_VERBOSE
    DEBUG_TRACE
    DEBUG_MEMORY
    DEBUG_TIMING
)
target_include_directories(test_debug_completo PRIVATE include)
target_include_directories(test_debug_completo PRIVATE ${CRITERION_INCLUDE_DIR})
target_link_libraries(test_debug_completo ${CRITERION_LIB})

# Tests con todas las características
add_executable(test_todas_caracteristicas
    tests/test_compilacion_condicional.c
    src/compilacion_condicional.c
)
target_compile_definitions(test_todas_caracteristicas PRIVATE 
    DEBUG
    DEBUG_VERBOSE
    DEBUG_TRACE
    DEBUG_MEMORY
    DEBUG_TIMING
    FEATURE_LOGGING
    FEATURE_STATS
    FEATURE_PROFILING
)
target_include_directories(test_todas_caracteristicas PRIVATE include)
target_include_directories(test_todas_caracteristicas PRIVATE ${CRITERION_INCLUDE_DIR})
target_link_libraries(test_todas_caracteristicas ${CRITERION_LIB})

# Habilitar testing
enable_testing()

# Agregar tests
add_test(NAME test_sin_debug COMMAND test_sin_debug)
add_test(NAME test_con_debug COMMAND test_con_debug)
add_test(NAME test_debug_completo COMMAND test_debug_completo)
add_test(NAME test_todas_caracteristicas COMMAND test_todas_caracteristicas)

# ============================================================================
# TARGETS PERSONALIZADOS
# ============================================================================

# Target para ejecutar ejemplo básico del enunciado
add_custom_target(demo_enunciado
    COMMAND echo "=== SIN DEBUG ===" && ${CMAKE_BINARY_DIR}/main_sin_debug
    COMMAND echo "" && echo "=== CON DEBUG ===" && ${CMAKE_BINARY_DIR}/main_con_debug
    DEPENDS main_sin_debug main_con_debug
    COMMENT "Demostrando el ejemplo del enunciado con y sin DEBUG"
)

# Target para comparar diferentes configuraciones
add_custom_target(comparar_configuraciones
    COMMAND echo "╔══════════════════════════════════════════════════════════════════╗"
    COMMAND echo "║                    COMPARANDO CONFIGURACIONES                    ║"
    COMMAND echo "╚══════════════════════════════════════════════════════════════════╝"
    COMMAND echo ""
    COMMAND echo "1. CONFIGURACIÓN MÍNIMA:"
    COMMAND ${CMAKE_BINARY_DIR}/demo_minimo | head -20
    COMMAND echo ""
    COMMAND echo "2. CON DEBUG BÁSICO:"
    COMMAND ${CMAKE_BINARY_DIR}/demo_debug_basico | head -20
    COMMAND echo ""
    COMMAND echo "3. CON DEBUG COMPLETO:"
    COMMAND ${CMAKE_BINARY_DIR}/demo_debug_completo | head -20
    DEPENDS demo_minimo demo_debug_basico demo_debug_completo
    COMMENT "Comparando diferentes niveles de configuración"
)

# Target para ejecutar todos los tests
add_custom_target(test_todas_configuraciones
    COMMAND echo "=== TESTS SIN DEBUG ===" && ${CMAKE_BINARY_DIR}/test_sin_debug --quiet
    COMMAND echo "=== TESTS CON DEBUG ===" && ${CMAKE_BINARY_DIR}/test_con_debug --quiet
    COMMAND echo "=== TESTS DEBUG COMPLETO ===" && ${CMAKE_BINARY_DIR}/test_debug_completo --quiet
    COMMAND echo "=== TESTS TODAS LAS CARACTERÍSTICAS ===" && ${CMAKE_BINARY_DIR}/test_todas_caracteristicas --quiet
    DEPENDS test_sin_debug test_con_debug test_debug_completo test_todas_caracteristicas
    COMMENT "Ejecutando tests con todas las configuraciones"
)

# Target para mostrar diferencias en el preprocesador
add_custom_target(mostrar_expansion_condicional
    COMMAND echo "=== EXPANSIÓN SIN DEBUG ==="
    COMMAND ${CMAKE_C_COMPILER} -E ${CMAKE_CURRENT_SOURCE_DIR}/src/compilacion_condicional.c -I${CMAKE_CURRENT_SOURCE_DIR}/include -DCOMPILAR_MAIN_BASICO | grep -A 5 -B 5 "procesarDatos\\|procesar_datos" | head -20
    COMMAND echo ""
    COMMAND echo "=== EXPANSIÓN CON DEBUG ==="
    COMMAND ${CMAKE_C_COMPILER} -E ${CMAKE_CURRENT_SOURCE_DIR}/src/compilacion_condicional.c -I${CMAKE_CURRENT_SOURCE_DIR}/include -DCOMPILAR_MAIN_BASICO -DDEBUG | grep -A 5 -B 5 "procesarDatos\\|procesar_datos" | head -20
    COMMENT "Mostrando diferencias en la expansión del preprocesador"
)

# Target para análisis de tamaño de ejecutables
add_custom_target(analizar_tamanos
    COMMAND echo "Analizando tamaños de ejecutables:"
    COMMAND echo "Sin debug:" && ls -lh ${CMAKE_BINARY_DIR}/main_sin_debug | awk '{print $$5}'
    COMMAND echo "Con debug:" && ls -lh ${CMAKE_BINARY_DIR}/main_con_debug | awk '{print $$5}'
    COMMAND echo "Producción:" && ls -lh ${CMAKE_BINARY_DIR}/demo_produccion | awk '{print $$5}'
    COMMAND echo "Todas características:" && ls -lh ${CMAKE_BINARY_DIR}/demo_todas_caracteristicas | awk '{print $$5}'
    DEPENDS main_sin_debug main_con_debug demo_produccion demo_todas_caracteristicas
    COMMENT "Analizando tamaños de diferentes configuraciones"
)

# Target para benchmark de rendimiento
add_custom_target(benchmark_configuraciones
    COMMAND echo "=== BENCHMARK DE CONFIGURACIONES ==="
    COMMAND echo "1. Sin debug (optimizado):"
    COMMAND time ${CMAKE_BINARY_DIR}/demo_produccion > /dev/null
    COMMAND echo "2. Con debug básico:"
    COMMAND time ${CMAKE_BINARY_DIR}/demo_debug_basico > /dev/null
    COMMAND echo "3. Con todas las características:"
    COMMAND time ${CMAKE_BINARY_DIR}/demo_todas_caracteristicas > /dev/null
    DEPENDS demo_produccion demo_debug_basico demo_todas_caracteristicas
    COMMENT "Comparando rendimiento de diferentes configuraciones"
)

# Target educativo principal
add_custom_target(tutorial_compilacion_condicional
    COMMAND echo "╔══════════════════════════════════════════════════════════════════╗"
    COMMAND echo "║              TUTORIAL: COMPILACIÓN CONDICIONAL                  ║"
    COMMAND echo "╚══════════════════════════════════════════════════════════════════╝"
    COMMAND echo ""
    COMMAND echo "1. Ejemplo básico del enunciado:"
    COMMAND echo "   make demo_enunciado"
    COMMAND echo ""
    COMMAND echo "2. Comparar configuraciones:"
    COMMAND echo "   make comparar_configuraciones"
    COMMAND echo ""
    COMMAND echo "3. Ver expansión del preprocesador:"
    COMMAND echo "   make mostrar_expansion_condicional"
    COMMAND echo ""
    COMMAND echo "4. Ejecutar todos los tests:"
    COMMAND echo "   make test_todas_configuraciones"
    COMMAND echo ""
    COMMAND echo "5. Analizar tamaños:"
    COMMAND echo "   make analizar_tamanos"
    COMMAND echo ""
    COMMAND echo "6. Benchmark de rendimiento:"
    COMMAND echo "   make benchmark_configuraciones"
    COMMAND echo ""
    COMMAND echo "Ejecutables disponibles:"
    COMMAND echo "  ./main_sin_debug           - Ejemplo sin DEBUG"
    COMMAND echo "  ./main_con_debug           - Ejemplo con DEBUG"
    COMMAND echo "  ./demo_minimo              - Demo configuración mínima"
    COMMAND echo "  ./demo_debug_completo      - Demo con DEBUG completo"
    COMMAND echo "  ./demo_todas_caracteristicas - Demo con todas las características"
    COMMAND echo "  ./demo_produccion          - Demo optimizado para producción"
    COMMENT "Tutorial interactivo de compilación condicional"
)

# ============================================================================
# CONFIGURACIONES PREDEFINIDAS
# ============================================================================

# Target para configuración de desarrollo
add_custom_target(config_desarrollo
    COMMAND ${CMAKE_COMMAND} -DENABLE_DEBUG=ON -DENABLE_DEBUG_VERBOSE=ON -DENABLE_FEATURE_LOGGING=ON ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Configurando para desarrollo (DEBUG + VERBOSE + LOGGING)"
)

# Target para configuración de testing
add_custom_target(config_testing
    COMMAND ${CMAKE_COMMAND} -DENABLE_DEBUG=ON -DENABLE_DEBUG_TRACE=ON -DENABLE_FEATURE_STATS=ON ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Configurando para testing (DEBUG + TRACE + STATS)"
)

# Target para configuración de producción
add_custom_target(config_produccion
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Configurando para producción (Release, sin DEBUG)"
)

# ============================================================================
# INSTALACIÓN
# ============================================================================

# Instalar biblioteca
install(TARGETS compilacion_condicional
        ARCHIVE DESTINATION lib)

# Instalar headers
install(FILES include/compilacion_condicional.h
        DESTINATION include)

# Instalar ejecutable principal
install(TARGETS main_sin_debug main_con_debug
        RUNTIME DESTINATION bin)

# ============================================================================
# INFORMACIÓN DE BUILD
# ============================================================================

# Mostrar configuración actual
message(STATUS "========================================")
message(STATUS "Configuración de Compilación Condicional:")
message(STATUS "  Estándar C: C${CMAKE_C_STANDARD}")
message(STATUS "  Tipo de build: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compilador: ${CMAKE_C_COMPILER}")

message(STATUS "")
message(STATUS "Opciones de DEBUG:")
message(STATUS "  ENABLE_DEBUG: ${ENABLE_DEBUG}")
message(STATUS "  ENABLE_DEBUG_VERBOSE: ${ENABLE_DEBUG_VERBOSE}")
message(STATUS "  ENABLE_DEBUG_TRACE: ${ENABLE_DEBUG_TRACE}")
message(STATUS "  ENABLE_DEBUG_MEMORY: ${ENABLE_DEBUG_MEMORY}")
message(STATUS "  ENABLE_DEBUG_TIMING: ${ENABLE_DEBUG_TIMING}")

message(STATUS "")
message(STATUS "Características opcionales:")
message(STATUS "  ENABLE_FEATURE_LOGGING: ${ENABLE_FEATURE_LOGGING}")
message(STATUS "  ENABLE_FEATURE_STATS: ${ENABLE_FEATURE_STATS}")
message(STATUS "  ENABLE_FEATURE_PROFILING: ${ENABLE_FEATURE_PROFILING}")

message(STATUS "")
message(STATUS "Targets principales:")
message(STATUS "  make demo_enunciado")
message(STATUS "  make comparar_configuraciones") 
message(STATUS "  make tutorial_compilacion_condicional")
message(STATUS "  make test_todas_configuraciones")
message(STATUS "========================================")

# ============================================================================
# DOCUMENTACIÓN
# ============================================================================

find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
    set(DOXYGEN_PROJECT_NAME "Compilación Condicional")
    set(DOXYGEN_PROJECT_BRIEF "Ejercicio educativo sobre #ifdef y compilación condicional")
    
    doxygen_add_docs(docs
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        COMMENT "Generando documentación con Doxygen"
    )
    
    message(STATUS "Doxygen encontrado - target 'docs' disponible")
endif()
