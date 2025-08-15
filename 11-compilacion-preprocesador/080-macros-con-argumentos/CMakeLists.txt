cmake_minimum_required(VERSION 3.10)
project(macros_con_argumentos C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Opciones de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -DDEBUG=1")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -DNDEBUG")

# Habilitar extensiones GNU para macros avanzadas
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

# Directorios de inclusión
include_directories(include)

# ============================================================================
# BIBLIOTECAS
# ============================================================================

# Biblioteca principal
add_library(macros_con_argumentos STATIC
    src/macros_con_argumentos.c
)

target_include_directories(macros_con_argumentos PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Enlazar libm para funciones matemáticas
target_link_libraries(macros_con_argumentos m)

# ============================================================================
# EJECUTABLES
# ============================================================================

# Ejecutable principal (función main_usuario_basico del enunciado)
add_executable(main_basico
    src/macros_con_argumentos.c
)

target_compile_definitions(main_basico PRIVATE COMPILAR_MAIN_BASICO)
target_include_directories(main_basico PRIVATE include)
target_link_libraries(main_basico m)

# Ejecutable de demostración completa
add_executable(demo_completo
    src/macros_con_argumentos.c
)

target_compile_definitions(demo_completo PRIVATE COMPILAR_DEMO_COMPLETO)
target_include_directories(demo_completo PRIVATE include)
target_link_libraries(demo_completo m)

# Ejecutable con información de debug
add_executable(demo_debug
    src/macros_con_argumentos.c
)

target_compile_definitions(demo_debug PRIVATE 
    COMPILAR_DEMO_COMPLETO
    DEBUG=1
)
target_include_directories(demo_debug PRIVATE include)
target_link_libraries(demo_debug m)

# Ejecutable para demostrar efectos secundarios
add_executable(demo_efectos_secundarios
    src/macros_con_argumentos.c
)

target_compile_definitions(demo_efectos_secundarios PRIVATE 
    COMPILAR_DEMO_EFECTOS_SECUNDARIOS
)
target_include_directories(demo_efectos_secundarios PRIVATE include)
target_link_libraries(demo_efectos_secundarios m)

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
add_executable(test_macros_con_argumentos
    tests/test_macros_con_argumentos.c
    src/macros_con_argumentos.c
)

target_include_directories(test_macros_con_argumentos PRIVATE
    include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CRITERION_INCLUDE_DIR}
)

target_link_libraries(test_macros_con_argumentos 
    ${CRITERION_LIB}
    m  # libm para funciones matemáticas
)

# Habilitar testing
enable_testing()

# Agregar test principal
add_test(NAME test_macros_con_argumentos 
         COMMAND test_macros_con_argumentos)

# ============================================================================
# TARGETS PERSONALIZADOS
# ============================================================================

# Target para ejecutar tests con salida detallada
add_custom_target(test_verbose
    COMMAND ${CMAKE_BINARY_DIR}/test_macros_con_argumentos --verbose
    DEPENDS test_macros_con_argumentos
    COMMENT "Ejecutando tests con salida detallada"
)

# Target para ejecutar tests específicos
add_custom_target(test_basicos
    COMMAND ${CMAKE_BINARY_DIR}/test_macros_con_argumentos --filter macros_basicas*
    DEPENDS test_macros_con_argumentos
    COMMENT "Ejecutando tests básicos de macros"
)

add_custom_target(test_efectos
    COMMAND ${CMAKE_BINARY_DIR}/test_macros_con_argumentos --filter efectos_secundarios*
    DEPENDS test_macros_con_argumentos
    COMMENT "Ejecutando tests de efectos secundarios"
)

add_custom_target(test_problematicas
    COMMAND ${CMAKE_BINARY_DIR}/test_macros_con_argumentos --filter macros_problematicas*
    DEPENDS test_macros_con_argumentos
    COMMENT "Ejecutando tests de macros problemáticas"
)

add_custom_target(test_educativos
    COMMAND ${CMAKE_BINARY_DIR}/test_macros_con_argumentos --filter educativo*
    DEPENDS test_macros_con_argumentos
    COMMENT "Ejecutando tests educativos"
)

# Target para demostrar el enunciado específico
add_custom_target(demo_enunciado
    COMMAND echo "=== EJECUTANDO EJEMPLO DEL ENUNCIADO ===" && ${CMAKE_BINARY_DIR}/main_basico
    DEPENDS main_basico
    COMMENT "Ejecutando ejemplo específico del enunciado"
)

# Target para comparar macros vs funciones
add_custom_target(demo_comparacion
    COMMAND echo "=== DEMOSTRACIÓN COMPLETA ===" && ${CMAKE_BINARY_DIR}/demo_completo
    DEPENDS demo_completo
    COMMENT "Demostrando diferencias entre macros y funciones"
)

# Target para mostrar expansión de preprocesador
add_custom_target(mostrar_expansion
    COMMAND echo "=== EXPANSIÓN DEL PREPROCESADOR ===" && ${CMAKE_C_COMPILER} -E ${CMAKE_CURRENT_SOURCE_DIR}/src/macros_con_argumentos.c -I${CMAKE_CURRENT_SOURCE_DIR}/include -DCOMPILAR_MAIN_BASICO | grep -A 10 -B 5 "CUADRADO"
    COMMENT "Mostrando expansión de macros por el preprocesador"
)

# Target para mostrar diferencias de compilación con optimizaciones
add_custom_target(demo_optimizacion
    COMMAND echo "=== SIN OPTIMIZACIÓN ===" && ${CMAKE_C_COMPILER} -O0 -S ${CMAKE_CURRENT_SOURCE_DIR}/src/macros_con_argumentos.c -I${CMAKE_CURRENT_SOURCE_DIR}/include -DCOMPILAR_MAIN_BASICO -o /tmp/sin_opt.s
    COMMAND echo "=== CON OPTIMIZACIÓN ===" && ${CMAKE_C_COMPILER} -O2 -S ${CMAKE_CURRENT_SOURCE_DIR}/src/macros_con_argumentos.c -I${CMAKE_CURRENT_SOURCE_DIR}/include -DCOMPILAR_MAIN_BASICO -o /tmp/con_opt.s
    COMMAND echo "Archivos generados: /tmp/sin_opt.s y /tmp/con_opt.s"
    COMMENT "Generando código ensamblador con y sin optimización"
)

# Target para análisis detallado de macros
add_custom_target(analizar_macros
    COMMAND echo "=== ANÁLISIS DE MACROS ===" 
    COMMAND echo "1. Mostrando definiciones de macros:"
    COMMAND grep -n "define.*(" ${CMAKE_CURRENT_SOURCE_DIR}/include/macros_con_argumentos.h | head -10
    COMMAND echo ""
    COMMAND echo "2. Expansión básica:"
    COMMAND echo 'CUADRADO(5)' | ${CMAKE_C_COMPILER} -E -include ${CMAKE_CURRENT_SOURCE_DIR}/include/macros_con_argumentos.h -
    COMMENT "Analizando definiciones y expansiones de macros"
)

# Target para demostrar problemas comunes
add_custom_target(demo_problemas
    COMMAND echo "=== DEMOSTRANDO PROBLEMAS COMUNES ===" 
    COMMAND echo "Ejecutando tests de efectos secundarios y macros problemáticas:"
    COMMAND ${CMAKE_BINARY_DIR}/test_macros_con_argumentos --filter "*problematicas*|*efectos_secundarios*" --verbose
    DEPENDS test_macros_con_argumentos
    COMMENT "Demostrando problemas comunes con macros"
)

# ============================================================================
# TARGETS DE DOCUMENTACIÓN Y APRENDIZAJE
# ============================================================================

# Target para generar informe educativo
add_custom_target(informe_educativo
    COMMAND echo "╔══════════════════════════════════════════════════════════════════╗"
    COMMAND echo "║                    INFORME EDUCATIVO - MACROS                   ║"
    COMMAND echo "╚══════════════════════════════════════════════════════════════════╝"
    COMMAND echo ""
    COMMAND echo "1. CONCEPTOS CLAVE:"
    COMMAND echo "   - Las macros realizan sustitución textual"
    COMMAND echo "   - Los argumentos se evalúan múltiples veces"
    COMMAND echo "   - Siempre usar paréntesis para evitar problemas de precedencia"
    COMMAND echo "   - Cuidado con efectos secundarios"
    COMMAND echo ""
    COMMAND echo "2. EJECUTAR EJEMPLO BÁSICO:"
    COMMAND echo "   make demo_enunciado"
    COMMAND echo ""
    COMMAND echo "3. VER TODOS LOS TESTS:"
    COMMAND echo "   make test_verbose"
    COMMAND echo ""
    COMMAND echo "4. DEMOSTRAR PROBLEMAS:"
    COMMAND echo "   make demo_problemas"
    COMMAND echo ""
    COMMAND echo "5. ANALIZAR EXPANSIÓN:"
    COMMAND echo "   make mostrar_expansion"
    COMMENT "Generando informe educativo sobre macros"
)

# ============================================================================
# INSTALACIÓN
# ============================================================================

# Instalar biblioteca
install(TARGETS macros_con_argumentos
        ARCHIVE DESTINATION lib)

# Instalar headers
install(FILES include/macros_con_argumentos.h
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
message(STATUS "  Estándar C: C${CMAKE_C_STANDARD} (GNU extensions enabled)")
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
message(STATUS "Targets disponibles:")
message(STATUS "  make demo_enunciado     - Ejecutar ejemplo del enunciado")
message(STATUS "  make demo_completo      - Demostración completa")
message(STATUS "  make test_verbose       - Tests con salida detallada")
message(STATUS "  make demo_problemas     - Demostrar problemas comunes")
message(STATUS "  make mostrar_expansion  - Ver expansión del preprocesador")
message(STATUS "  make informe_educativo  - Generar informe educativo")
message(STATUS "========================================")

# ============================================================================
# DOCUMENTACIÓN
# ============================================================================

# Target para generar documentación con Doxygen si está disponible
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
    set(DOXYGEN_PROJECT_NAME "Macros con Argumentos")
    set(DOXYGEN_PROJECT_BRIEF "Ejercicio educativo sobre macros con parámetros en C")
    
    doxygen_add_docs(docs
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        COMMENT "Generando documentación con Doxygen"
    )
    
    message(STATUS "Doxygen encontrado - target 'docs' disponible")
else()
    message(STATUS "Doxygen no encontrado - documentación no disponible")
endif()
