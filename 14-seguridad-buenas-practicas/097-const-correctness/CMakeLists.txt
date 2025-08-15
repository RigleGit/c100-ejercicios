cmake_minimum_required(VERSION 3.10)
project(const_correctness VERSION 1.0 LANGUAGES C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Configuración de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")

# Configuración de directorios
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Directorios de inclusión
include_directories(include)

# Archivos fuente principales
set(SOURCES
    src/const_correctness.c
)

set(HEADERS
    include/const_correctness.h
)

# Biblioteca estática
add_library(const_correctness_lib STATIC ${SOURCES} ${HEADERS})
target_include_directories(const_correctness_lib PUBLIC include)

# Executable principal
add_executable(const_correctness src/main.c)
target_link_libraries(const_correctness const_correctness_lib)

# Configuración específica para const-correctness
target_compile_options(const_correctness_lib PRIVATE
    -Wwrite-strings           # String literals como const char*
    -Wcast-qual               # Advertencias sobre casts que eliminan const
    -Wpointer-arith           # Aritmética de punteros
    -Wstrict-prototypes       # Prototipos estrictos
    -Wmissing-prototypes      # Prototipos faltantes
    -Wredundant-decls         # Declaraciones redundantes
)

# Buscar Criterion para pruebas
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()

find_library(CRITERION_LIB criterion)
find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
if(CRITERION_LIB)
    message(STATUS "Criterion encontrado: ${CRITERION_LIB}")
    
    # Executable de pruebas
    add_executable(test_const_correctness tests/test_const_correctness.c)
    target_link_libraries(test_const_correctness 
        const_correctness_lib 
        ${CRITERION_LIB}
        ${CRITERION_LIBRARIES}
    )
    if (CRITERION_INCLUDE_DIR)
        target_include_directories(test_const_correctness PRIVATE ${CRITERION_INCLUDE_DIR})
    endif()
    if (CRITERION_INCLUDE_DIRS)
        target_include_directories(test_const_correctness PRIVATE ${CRITERION_INCLUDE_DIRS})
    endif()
    if (CRITERION_LIBRARY_DIRS)
        target_link_directories(test_const_correctness PRIVATE ${CRITERION_LIBRARY_DIRS})
    endif()
    if (CRITERION_LDFLAGS)
        target_link_options(test_const_correctness PRIVATE ${CRITERION_LDFLAGS})
    endif()
    
    # Configuración adicional para pruebas
    target_compile_options(test_const_correctness PRIVATE
        -DCRITERION_TESTING
        -g
    )
    target_compile_definitions(test_const_correctness PRIVATE UNIT_TESTING)
    
    # Integración con CTest
    enable_testing()
    add_test(NAME const_correctness_tests
             COMMAND test_const_correctness --verbose
             WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    # Target auxiliar para ejecutar desde build
    add_custom_target(run_tests
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS test_const_correctness
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Ejecutando suite de pruebas con CTest"
    )
    
    # Test de memoria con valgrind si está disponible
    find_program(VALGRIND_PROGRAM valgrind)
    if(VALGRIND_PROGRAM)
        add_custom_target(test_memory
            COMMAND ${VALGRIND_PROGRAM} 
                --tool=memcheck 
                --leak-check=full 
                --show-leak-kinds=all 
                --track-origins=yes 
                --verbose 
                --error-exitcode=1
                ./test_const_correctness
            DEPENDS test_const_correctness
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            COMMENT "Ejecutando pruebas con verificación de memoria"
        )
    endif()
    
else()
    message(WARNING "Criterion no encontrado. Las pruebas no estarán disponibles.")
    message(STATUS "Para instalar Criterion:")
    message(STATUS "  Ubuntu/Debian: sudo apt-get install libcriterion-dev")
    message(STATUS "  macOS: brew install criterion")
    message(STATUS "  Desde fuente: https://github.com/Snaipe/Criterion")
endif()

# Herramientas de análisis estático
find_program(CLANG_TIDY_PROGRAM clang-tidy)
if(CLANG_TIDY_PROGRAM)
    add_custom_target(lint
        COMMAND ${CLANG_TIDY_PROGRAM}
            src/*.c
            include/*.h
            --
            -Iinclude
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Ejecutando análisis estático con clang-tidy"
    )
endif()

find_program(CPPCHECK_PROGRAM cppcheck)
if(CPPCHECK_PROGRAM)
    add_custom_target(static_analysis
        COMMAND ${CPPCHECK_PROGRAM}
            --enable=all
            --std=c11
            --verbose
            --error-exitcode=1
            -I include
            src/
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Ejecutando análisis estático con cppcheck"
    )
endif()

# Herramientas de formato
find_program(CLANG_FORMAT_PROGRAM clang-format)
if(CLANG_FORMAT_PROGRAM)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_PROGRAM}
            -i
            -style="{BasedOnStyle: Linux, IndentWidth: 4, ColumnLimit: 100}"
            src/*.c
            include/*.h
            tests/*.c
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Formateando código con clang-format"
    )
endif()

# Target para generar documentación con Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(
        ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in
        ${CMAKE_BINARY_DIR}/Doxyfile
        @ONLY
    )
    
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generando documentación con Doxygen"
    )
endif()

# Configuración de instalación
include(GNUInstallDirs)

install(TARGETS const_correctness
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS const_correctness_lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/const_correctness
)

install(FILES README.md
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

# Configuración de empaquetado
set(CPACK_PACKAGE_NAME "const-correctness")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ejercicio de const-correctness en C")
set(CPACK_PACKAGE_VENDOR "Curso C")
set(CPACK_PACKAGE_CONTACT "curso-c@ejemplo.com")
# El LICENSE no existe en el repo; evitar fallo de CPack
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

# Configuración específica para diferentes generadores de paquetes
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.17)")
set(CPACK_RPM_PACKAGE_REQUIRES "glibc >= 2.17")

include(CPack)

# Información de configuración
message(STATUS "=== Configuración del Proyecto ===")
message(STATUS "Proyecto: ${PROJECT_NAME}")
message(STATUS "Versión: ${PROJECT_VERSION}")
message(STATUS "Compilador C: ${CMAKE_C_COMPILER}")
message(STATUS "Flags de C: ${CMAKE_C_FLAGS}")
message(STATUS "Tipo de compilación: ${CMAKE_BUILD_TYPE}")
message(STATUS "Directorio de salida: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

if(CRITERION_LIB)
    message(STATUS "Pruebas: Habilitadas (Criterion)")
else()
    message(STATUS "Pruebas: Deshabilitadas (Criterion no encontrado)")
endif()

# Targets principales disponibles
message(STATUS "=== Targets Disponibles ===")
message(STATUS "  const_correctness     - Programa principal")
if(CRITERION_LIB)
    message(STATUS "  test_const_correctness - Suite de pruebas")
    message(STATUS "  run_tests            - Ejecutar pruebas con timeout")
    if(VALGRIND_PROGRAM)
        message(STATUS "  test_memory          - Pruebas con valgrind")
    endif()
endif()

if(CLANG_TIDY_PROGRAM)
    message(STATUS "  lint                 - Análisis con clang-tidy")
endif()

if(CPPCHECK_PROGRAM)
    message(STATUS "  static_analysis      - Análisis con cppcheck")
endif()

if(CLANG_FORMAT_PROGRAM)
    message(STATUS "  format               - Formatear código")
endif()

if(DOXYGEN_FOUND)
    message(STATUS "  docs                 - Generar documentación")
endif()

message(STATUS "=====================================")

# Verificaciones adicionales para const-correctness
include(CheckCCompilerFlag)

# Verificar soporte para flags específicos de const
check_c_compiler_flag("-Wcast-qual" HAS_CAST_QUAL_WARNING)
check_c_compiler_flag("-Wwrite-strings" HAS_WRITE_STRINGS_WARNING)

if(HAS_CAST_QUAL_WARNING)
    message(STATUS "Soporte para -Wcast-qual: Sí")
else()
    message(WARNING "Soporte para -Wcast-qual: No")
endif()

if(HAS_WRITE_STRINGS_WARNING)
    message(STATUS "Soporte para -Wwrite-strings: Sí")
else()
    message(WARNING "Soporte para -Wwrite-strings: No")
endif()
