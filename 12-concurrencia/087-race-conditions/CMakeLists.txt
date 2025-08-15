cmake_minimum_required(VERSION 3.10)
project(RaceConditions VERSION 1.0.0 LANGUAGES C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Opciones de compilación
option(BUILD_TESTS "Build test programs" ON)
option(BUILD_EXAMPLES "Build example programs" ON)
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

# Configuración de banderas del compilador
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat=2 -Wno-unused-parameter")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow -Wwrite-strings -Wstrict-prototypes")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wold-style-definition -Wredundant-decls")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wnested-externs -Wmissing-include-dirs")

# Configuración Debug/Release
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3 -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")

# Configuración para diferentes compiladores
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wlogical-op -Wduplicated-cond")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wduplicated-branches -Wnull-dereference")
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-field-initializers")
endif()

# Sanitizers
if(ENABLE_ASAN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

if(ENABLE_TSAN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
endif()

# Coverage
if(ENABLE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# Encontrar dependencias requeridas
find_package(Threads REQUIRED)

# Verificar disponibilidad de pthread
include(CheckIncludeFile)
check_include_file("pthread.h" HAVE_PTHREAD_H)
if(NOT HAVE_PTHREAD_H)
    message(FATAL_ERROR "pthread.h is required but not found")
endif()

# Verificar funciones pthread específicas
include(CheckFunctionExists)
set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
check_function_exists(pthread_create HAVE_PTHREAD_CREATE)
check_function_exists(pthread_mutex_init HAVE_PTHREAD_MUTEX_INIT)
if(NOT HAVE_PTHREAD_CREATE OR NOT HAVE_PTHREAD_MUTEX_INIT)
    message(FATAL_ERROR "Required pthread functions not found")
endif()

# Configurar directorio de inclusión
include_directories(include)

# Crear directorio de salida para binarios
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ============================================================================
# BIBLIOTECA PRINCIPAL
# ============================================================================

# Fuentes de la biblioteca
set(LIB_SOURCES
    src/race_conditions.c
)

# Headers públicos
set(LIB_HEADERS
    include/race_conditions.h
)

# Crear biblioteca estática
add_library(race_conditions_static STATIC ${LIB_SOURCES})
target_link_libraries(race_conditions_static Threads::Threads)
target_compile_definitions(race_conditions_static PRIVATE _GNU_SOURCE)

# Crear biblioteca compartida
add_library(race_conditions_shared SHARED ${LIB_SOURCES})
target_link_libraries(race_conditions_shared Threads::Threads)
target_compile_definitions(race_conditions_shared PRIVATE _GNU_SOURCE)
set_target_properties(race_conditions_shared PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME race_conditions
)

# Alias para facilidad de uso
add_library(RaceConditions::Static ALIAS race_conditions_static)
add_library(RaceConditions::Shared ALIAS race_conditions_shared)

# ============================================================================
# PROGRAMA PRINCIPAL
# ============================================================================

if(BUILD_EXAMPLES)
    # Programa principal interactivo
    add_executable(race_conditions_main src/main.c)
    target_link_libraries(race_conditions_main race_conditions_static)
    set_target_properties(race_conditions_main PROPERTIES OUTPUT_NAME race_conditions)
    
    # Programa de demostración simple
    add_executable(race_conditions_demo)
    target_sources(race_conditions_demo PRIVATE
        demo/demo_simple.c
        src/race_conditions.c
    )
    target_link_libraries(race_conditions_demo Threads::Threads)
    target_compile_definitions(race_conditions_demo PRIVATE _GNU_SOURCE)
endif()

# ============================================================================
# TESTS
# ============================================================================

if(BUILD_TESTS)
    # Buscar Criterion para tests
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(CRITERION criterion)
    endif()
    
    if(NOT CRITERION_FOUND)
        # Buscar Criterion manualmente
        find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
        find_library(CRITERION_LIBRARY criterion)
        
        if(CRITERION_INCLUDE_DIR AND CRITERION_LIBRARY)
            set(CRITERION_FOUND TRUE)
            set(CRITERION_INCLUDE_DIRS ${CRITERION_INCLUDE_DIR})
            set(CRITERION_LIBRARIES ${CRITERION_LIBRARY})
        endif()
    endif()
    
    if(CRITERION_FOUND)
        message(STATUS "Criterion found - building tests")
        
        # Configurar tests
        enable_testing()
        
        # Test principal
        add_executable(test_race_conditions tests/test_race_conditions.c)
        target_link_libraries(test_race_conditions 
            race_conditions_static 
            ${CRITERION_LIBRARIES}
        )
        if(CRITERION_LIBRARY_DIRS)
            target_link_directories(test_race_conditions PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_definitions(test_race_conditions PRIVATE UNIT_TESTING)
        endif()
        target_include_directories(test_race_conditions PRIVATE ${CRITERION_INCLUDE_DIRS})
        
        # Configuración específica para tests con ThreadSanitizer
        if(ENABLE_TSAN)
            target_compile_definitions(test_race_conditions PRIVATE TSAN_ENABLED)
        endif()
        
        # Tests específicos para diferentes aspectos
        add_test(NAME test_race_conditions_basicos 
                COMMAND test_race_conditions --pattern="race_conditions_basicos/*")
        add_test(NAME test_race_conditions_config 
                COMMAND test_race_conditions --pattern="race_conditions_config/*")
        add_test(NAME test_race_conditions_estadistico 
                COMMAND test_race_conditions --pattern="race_conditions_estadistico/*")
        add_test(NAME test_race_conditions_rendimiento 
                COMMAND test_race_conditions --pattern="race_conditions_rendimiento/*")
        add_test(NAME test_race_conditions_utilidades 
                COMMAND test_race_conditions --pattern="race_conditions_utilidades/*")
        add_test(NAME test_race_conditions_robustez 
                COMMAND test_race_conditions --pattern="race_conditions_robustez/*")
        
        # Test que ejecuta todos
        add_test(NAME test_race_conditions_all COMMAND test_race_conditions)
        
        # Configurar timeout para tests (importante para concurrencia)
        set_tests_properties(test_race_conditions_all PROPERTIES TIMEOUT 300)
        set_tests_properties(test_race_conditions_rendimiento PROPERTIES TIMEOUT 120)
        
        # Tests de benchmark
        add_executable(benchmark_race_conditions tests/benchmark_race_conditions.c)
        target_link_libraries(benchmark_race_conditions race_conditions_static)
        
        # Tests de estrés
        add_executable(stress_test_race_conditions tests/stress_test.c)
        target_link_libraries(stress_test_race_conditions race_conditions_static)
        
    else()
        message(WARNING "Criterion not found - tests will not be built")
        message(STATUS "To install Criterion on macOS: brew install criterion")
        message(STATUS "To install Criterion on Ubuntu: sudo apt-get install libcriterion-dev")
    endif()
endif()

# ============================================================================
# HERRAMIENTAS Y UTILIDADES
# ============================================================================

# Programa para generar reportes
add_executable(race_conditions_reporter tools/reporter.c)
target_link_libraries(race_conditions_reporter race_conditions_static)

# Programa para análisis automatizado
add_executable(race_conditions_analyzer tools/analyzer.c)
target_link_libraries(race_conditions_analyzer race_conditions_static)

# ============================================================================
# DOCUMENTACIÓN
# ============================================================================

# Buscar Doxygen para documentación
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    set(DOXYGEN_INPUT_DIR ${CMAKE_SOURCE_DIR}/include)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
    
    configure_file(${CMAKE_SOURCE_DIR}/docs/Doxyfile.in 
                   ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
    
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# ============================================================================
# INSTALACIÓN
# ============================================================================

# Instalar bibliotecas
install(TARGETS race_conditions_static race_conditions_shared
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Instalar headers
install(FILES ${LIB_HEADERS} DESTINATION include)

# Instalar programas principales
if(BUILD_EXAMPLES)
    install(TARGETS race_conditions_main DESTINATION bin)
endif()

# Instalar documentación
install(FILES README.md DESTINATION share/doc/race_conditions)

# ============================================================================
# EMPAQUETADO
# ============================================================================

set(CPACK_PACKAGE_NAME "race_conditions")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Race Conditions Demonstration Library")
set(CPACK_PACKAGE_VENDOR "C100 Ejercicios")
set(CPACK_PACKAGE_CONTACT "educacion@c100ejercicios.com")

include(CPack)

# ============================================================================
# CUSTOM TARGETS
# ============================================================================

# Target para ejecutar todos los tests con verbose
add_custom_target(test_verbose
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS test_race_conditions
    COMMENT "Running all tests with verbose output"
)

# Target para ejecutar análisis de race conditions
add_custom_target(analyze_races
    COMMAND $<TARGET_FILE:race_conditions_analyzer>
    DEPENDS race_conditions_analyzer
    COMMENT "Running race condition analysis"
)

# Target para limpiar todos los archivos generados
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/docs
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/coverage
    COMMENT "Cleaning all generated files"
)

# Target para ejecutar con ThreadSanitizer
if(ENABLE_TSAN)
    add_custom_target(run_with_tsan
        COMMAND TSAN_OPTIONS=verbosity=1:halt_on_error=1 $<TARGET_FILE:race_conditions_main>
        DEPENDS race_conditions_main
        COMMENT "Running main program with ThreadSanitizer"
    )
endif()

# ============================================================================
# INFORMACIÓN DE CONFIGURACIÓN
# ============================================================================

message(STATUS "")
message(STATUS "===== Race Conditions Configuration =====")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Build examples: ${BUILD_EXAMPLES}")
message(STATUS "Enable AddressSanitizer: ${ENABLE_ASAN}")
message(STATUS "Enable ThreadSanitizer: ${ENABLE_TSAN}")
message(STATUS "Enable Coverage: ${ENABLE_COVERAGE}")
if(CRITERION_FOUND)
    message(STATUS "Criterion: Found")
else()
    message(STATUS "Criterion: Not found (tests disabled)")
endif()
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "==========================================")
message(STATUS "")
