cmake_minimum_required(VERSION 3.10)

# Configuración del proyecto
project(ejercicio_030_empaquetar_bytes 
    VERSION 1.0.0 
    DESCRIPTION "Ejercicio 030: Empaquetar Bytes - Operaciones a Nivel de Bit"
    LANGUAGES C
)

# Estándar de C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Opciones de compilación
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")

# Directorios
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Incluir directorio de headers si existe
if(EXISTS "${INCLUDE_DIR}")
    include_directories("${INCLUDE_DIR}")
endif()

# Executable principal
add_executable(ejercicio_030
    "${SRC_DIR}/main.c"
)

# Propiedades del ejecutable principal
set_target_properties(ejercicio_030 PROPERTIES
    OUTPUT_NAME "empaquetar_bytes"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Executable de tests
if(EXISTS "${TEST_DIR}/test_main.c")
    add_executable(test_ejercicio_030
        "${TEST_DIR}/test_main.c"
    )
    
    set_target_properties(test_ejercicio_030 PROPERTIES
        OUTPUT_NAME "test_empaquetar_bytes"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

# Target personalizado para ejecutar el programa principal
add_custom_target(run_main
    COMMAND $<TARGET_FILE:ejercicio_030>
    DEPENDS ejercicio_030
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Ejecutando ejercicio 030: Empaquetar Bytes"
)

# Target personalizado para ejecutar tests
if(TARGET test_ejercicio_030)
    add_custom_target(run_tests
        COMMAND $<TARGET_FILE:test_ejercicio_030>
        DEPENDS test_ejercicio_030
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Ejecutando tests del ejercicio 030"
    )
endif()

# Target para ejecutar ambos
add_custom_target(run_all
    COMMENT "Ejecutando programa principal y tests"
)

if(TARGET test_ejercicio_030)
    add_dependencies(run_all ejercicio_030 test_ejercicio_030)
    add_custom_command(TARGET run_all POST_BUILD
        COMMAND echo "=== Ejecutando Tests ==="
        COMMAND $<TARGET_FILE:test_ejercicio_030>
        COMMAND echo ""
        COMMAND echo "=== Ejecutando Programa Principal ==="
        COMMAND echo "100" | $<TARGET_FILE:ejercicio_030>
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
else()
    add_dependencies(run_all ejercicio_030)
    add_custom_command(TARGET run_all POST_BUILD
        COMMAND $<TARGET_FILE:ejercicio_030>
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()

# Información del build
message(STATUS "=== Configuración del Ejercicio 030: Empaquetar Bytes ===")
message(STATUS "Versión: ${PROJECT_VERSION}")
message(STATUS "Compilador C: ${CMAKE_C_COMPILER_ID}")
message(STATUS "Estándar C: C${CMAKE_C_STANDARD}")
message(STATUS "Tipo de build: ${CMAKE_BUILD_TYPE}")
message(STATUS "Directorio fuente: ${SRC_DIR}")
message(STATUS "Directorio tests: ${TEST_DIR}")
message(STATUS "Directorio include: ${INCLUDE_DIR}")

# Detectar arquitectura para información sobre endianness
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Arquitectura: 64-bit")
else()
    message(STATUS "Arquitectura: 32-bit")
endif()

# Verificar el sistema para notas sobre portabilidad
if(WIN32)
    message(STATUS "Sistema: Windows")
elseif(APPLE)
    message(STATUS "Sistema: macOS")
elseif(UNIX)
    message(STATUS "Sistema: Unix/Linux")
endif()

message(STATUS "========================================================")

# Instalar archivos
install(TARGETS ejercicio_030
    RUNTIME DESTINATION bin
    COMPONENT runtime
)

if(TARGET test_ejercicio_030)
    install(TARGETS test_ejercicio_030
        RUNTIME DESTINATION bin
        COMPONENT testing
    )
endif()

# Instalar documentación
install(FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    DESTINATION doc/ejercicio_030
    COMPONENT documentation
)

# Configuración para empaquetado
set(CPACK_PACKAGE_NAME "ejercicio_030_empaquetar_bytes")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR "Curso C - 100 Ejercicios")

include(CPack)
