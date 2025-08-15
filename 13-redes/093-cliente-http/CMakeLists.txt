cmake_minimum_required(VERSION 3.10)
project(ClienteHTTP VERSION 1.0.0 LANGUAGES C)

# Configuración del estándar C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Configuración de compilación
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

# Flags de compilación comunes
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat=2 -Wformat-security")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wnull-dereference -Wstack-protector")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-overflow=5 -Warray-bounds=2")

# Configuración específica por compilador
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector-strong")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wlogical-op -Wduplicated-cond")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wduplicated-branches")
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wthread-safety")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wconditional-uninitialized")
endif()

# Directorios de headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Buscar dependencias del sistema
find_package(Threads REQUIRED)

# Configuración para diferentes plataformas
if(UNIX AND NOT APPLE)
    # Linux específico
    add_definitions(-D_GNU_SOURCE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FORTIFY_SOURCE=2")
elseif(APPLE)
    # macOS específico
    add_definitions(-D_DARWIN_C_SOURCE)
elseif(WIN32)
    # Windows específico
    add_definitions(-D_WIN32_WINNT=0x0601)
    set(PLATFORM_LIBS ws2_32)
endif()

# Fuentes principales
set(CLIENTE_HTTP_SOURCES
    src/cliente_http.c
)

set(CLIENTE_HTTP_HEADERS
    include/cliente_http.h
)

# Executable principal
add_executable(cliente_http
    ${CLIENTE_HTTP_SOURCES}
    src/main.c
)

target_link_libraries(cliente_http 
    ${PLATFORM_LIBS}
)

# Herramientas de desarrollo
add_executable(simple_http_client
    ${CLIENTE_HTTP_SOURCES}
    tools/simple_http_client.c
)

target_link_libraries(simple_http_client
    ${PLATFORM_LIBS}
)

add_executable(http_benchmark
    ${CLIENTE_HTTP_SOURCES}
    tools/http_benchmark.c
)

target_link_libraries(http_benchmark
    ${PLATFORM_LIBS}
)

# Configuración de tests con Criterion
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(CRITERION criterion)
endif()
if(NOT CRITERION_FOUND)
    find_library(CRITERION_LIBRARY criterion)
    find_path(CRITERION_INCLUDE_DIR criterion/criterion.h)
    if(CRITERION_LIBRARY AND CRITERION_INCLUDE_DIR)
        set(CRITERION_FOUND TRUE)
        set(CRITERION_LIBRARIES ${CRITERION_LIBRARY})
        set(CRITERION_INCLUDE_DIRS ${CRITERION_INCLUDE_DIR})
    endif()
endif()
if(CRITERION_FOUND)
    enable_testing()
    
    add_executable(test_cliente_http
        ${CLIENTE_HTTP_SOURCES}
        tests/test_cliente_http.c
    )
    target_include_directories(test_cliente_http PRIVATE ${CRITERION_INCLUDE_DIRS})
    if(CRITERION_LIBRARY_DIRS)
        target_link_directories(test_cliente_http PRIVATE ${CRITERION_LIBRARY_DIRS})
    target_compile_definitions(test_cliente_http PRIVATE UNIT_TESTING)
    endif()
    target_link_libraries(test_cliente_http
        ${CRITERION_LIBRARIES}
        Threads::Threads
        ${PLATFORM_LIBS}
    )
    
    add_test(NAME TestClienteHTTP
             COMMAND test_cliente_http)
    
    # Configuración adicional para tests
    set_property(TEST TestClienteHTTP
                PROPERTY ENVIRONMENT "CRITERION_ALWAYS_SUCCEED=0")
    
    message(STATUS "Tests habilitados con Criterion")
else()
    message(WARNING "Criterion no encontrado. Tests deshabilitados.")
    message(STATUS "Para habilitar tests, instala Criterion:")
    message(STATUS "  Ubuntu/Debian: sudo apt-get install libcriterion-dev")
    message(STATUS "  macOS: brew install criterion")
    message(STATUS "  Arch Linux: sudo pacman -S criterion")
endif()

# Configuración para herramientas de análisis
option(ENABLE_SANITIZERS "Habilitar sanitizers" OFF)
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SANITIZER_FLAGS "-fsanitize=address -fsanitize=undefined")
    set(SANITIZER_FLAGS "${SANITIZER_FLAGS} -fno-sanitize-recover=all")
    
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${SANITIZER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${SANITIZER_FLAGS}")
    
    message(STATUS "Sanitizers habilitados")
endif()

# Configuración para análisis estático
option(ENABLE_STATIC_ANALYSIS "Habilitar análisis estático" OFF)
if(ENABLE_STATIC_ANALYSIS)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if(CLANG_TIDY_EXE)
        set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY_EXE})
        message(STATUS "clang-tidy habilitado")
    endif()
    
    find_program(CPPCHECK_EXE NAMES "cppcheck")
    if(CPPCHECK_EXE)
        set(CMAKE_C_CPPCHECK ${CPPCHECK_EXE})
        message(STATUS "cppcheck habilitado")
    endif()
endif()

# Configuración para coverage
option(ENABLE_COVERAGE "Habilitar coverage" OFF)
if(ENABLE_COVERAGE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
    message(STATUS "Coverage habilitado")
endif()

# Configuración para documentación
find_program(DOXYGEN_EXE doxygen)
if(DOXYGEN_EXE)
    configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(docs
        ${DOXYGEN_EXE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generando documentación con Doxygen"
    )
endif()

# Configuración para empaquetado
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Cliente HTTP básico usando sockets TCP")
set(CPACK_PACKAGE_VENDOR "Ejercicios C")
set(CPACK_PACKAGE_CONTACT "ejercicios-c@example.com")
# Evitar error si faltan LICENSE/README
if(EXISTS ${CMAKE_SOURCE_DIR}/LICENSE)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
endif()
if(EXISTS ${CMAKE_SOURCE_DIR}/README.md)
    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
endif()

include(CPack)

# Targets personalizados
add_custom_target(format
    COMMAND find ${CMAKE_SOURCE_DIR} -name "*.c" -o -name "*.h" | 
            xargs clang-format -i -style=file
    COMMENT "Formateando código fuente"
)

add_custom_target(analyze
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMENT "Ejecutando análisis estático"
)

# Target para pruebas rápidas de conectividad
add_custom_target(test-connectivity
    COMMAND echo "Probando conectividad básica..."
    COMMAND ping -c 1 example.com || echo "Sin conectividad a example.com"
    COMMAND ping -c 1 httpbin.org || echo "Sin conectividad a httpbin.org"
    COMMENT "Verificando conectividad de red"
)

# Target para demostración
add_custom_target(demo
    COMMAND echo "=== Demo Cliente HTTP ==="
    COMMAND echo "Compilando..."
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target cliente_http
    COMMAND echo "Ejecutando demo con example.com..."
    COMMAND ${CMAKE_BINARY_DIR}/simple_http_client http://example.com
    COMMENT "Ejecutando demostración del cliente HTTP"
    DEPENDS simple_http_client
)

# Información de configuración
message(STATUS "")
message(STATUS "Configuración del proyecto:")
message(STATUS "  Nombre: ${PROJECT_NAME}")
message(STATUS "  Versión: ${PROJECT_VERSION}")
message(STATUS "  Compilador: ${CMAKE_C_COMPILER_ID}")
message(STATUS "  Tipo de build: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Flags: ${CMAKE_C_FLAGS}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "  Flags debug: ${CMAKE_C_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "  Flags release: ${CMAKE_C_FLAGS_RELEASE}")
endif()
message(STATUS "")

# Targets de utilidad
message(STATUS "Targets disponibles:")
message(STATUS "  make cliente_http           - Compilar cliente principal")
message(STATUS "  make simple_http_client     - Compilar cliente simple CLI")
message(STATUS "  make http_benchmark         - Compilar herramienta de benchmark")
if(CRITERION_LIB)
message(STATUS "  make test_cliente_http      - Compilar tests")
message(STATUS "  make test                   - Ejecutar tests")
endif()
if(DOXYGEN_EXE)
message(STATUS "  make docs                   - Generar documentación")
endif()
message(STATUS "  make format                 - Formatear código")
message(STATUS "  make test-connectivity      - Verificar conectividad")
message(STATUS "  make demo                   - Ejecutar demostración")
message(STATUS "  make package                - Crear paquete")
message(STATUS "")

# Configuración de instalación
install(TARGETS cliente_http simple_http_client http_benchmark
        RUNTIME DESTINATION bin)

install(FILES ${CLIENTE_HTTP_HEADERS}
        DESTINATION include)

install(FILES README.md
        DESTINATION share/doc/${PROJECT_NAME})

# Crear script de ejemplo
file(WRITE ${CMAKE_BINARY_DIR}/test_urls.txt
"# URLs de prueba para el cliente HTTP
http://example.com
http://httpbin.org/get
http://httpbin.org/headers
http://httpbin.org/user-agent
http://httpbin.org/status/200
")

install(FILES ${CMAKE_BINARY_DIR}/test_urls.txt
        DESTINATION share/doc/${PROJECT_NAME}/examples)
