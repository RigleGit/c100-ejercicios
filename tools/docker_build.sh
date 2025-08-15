#!/bin/bash
# docker_build.sh - Script para construir y gestionar el entorno Docker

set -e

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Función de ayuda
show_help() {
    echo "🐳 Script de gestión Docker para C100 Ejercicios"
    echo "================================================="
    echo ""
    echo "Uso: $0 [COMANDO] [OPCIONES]"
    echo ""
    echo "COMANDOS:"
    echo "  build          Construir la imagen Docker"
    echo "  run            Ejecutar contenedor interactivo"
    echo "  test           Ejecutar todos los tests en Docker"
    echo "  test-block N   Ejecutar tests del bloque N"
    echo "  clean          Limpiar contenedores e imágenes"
    echo "  logs           Ver logs del contenedor"
    echo "  shell          Abrir shell en contenedor existente"
    echo "  stop           Detener todos los contenedores"
    echo "  compose-up     Iniciar con docker-compose"
    echo "  compose-down   Detener docker-compose"
    echo ""
    echo "EJEMPLOS:"
    echo "  $0 build                    # Construir imagen"
    echo "  $0 run                      # Ejecutar contenedor"
    echo "  $0 test                     # Probar todos los ejercicios"
    echo "  $0 test-block 5             # Probar solo el bloque 5"
    echo "  $0 compose-up               # Levantar todos los servicios"
    echo ""
}

# Función para mostrar mensajes con color
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Verificar que Docker está disponible
check_docker() {
    if ! command -v docker &> /dev/null; then
        log_error "Docker no está instalado o no está en el PATH"
        exit 1
    fi
    
    if ! docker info &> /dev/null; then
        log_error "Docker no está ejecutándose"
        exit 1
    fi
}

# Construir imagen Docker
build_image() {
    log_info "Construyendo imagen Docker c100-ejercicios..."
    
    docker build -t c100-ejercicios:latest -f Dockerfile .
    
    if [ $? -eq 0 ]; then
        log_success "Imagen construida exitosamente"
    else
        log_error "Error construyendo la imagen"
        exit 1
    fi
}

# Ejecutar contenedor interactivo
run_container() {
    log_info "Ejecutando contenedor c100-ejercicios..."
    
    # Detener contenedor existente si está corriendo
    docker stop c100-dev 2>/dev/null || true
    docker rm c100-dev 2>/dev/null || true
    
    docker run -it \
        --name c100-dev \
        --hostname c100-dev \
        -v "$(pwd):/workspace" \
        -p 8080-8089:8080-8089 \
        -p 9999:9999 \
        --cap-add=SYS_PTRACE \
        --security-opt seccomp=unconfined \
        c100-ejercicios:latest
}

# Ejecutar tests en Docker
run_tests() {
    log_info "Ejecutando todos los tests en Docker..."
    
    docker run --rm \
        -v "$(pwd):/workspace" \
        c100-ejercicios:latest \
        bash -c "cd /workspace && ./tools/test_everything_fresh.sh"
}

# Ejecutar tests de un bloque específico
run_test_block() {
    local block=$1
    if [ -z "$block" ]; then
        log_error "Debe especificar el número de bloque"
        exit 1
    fi
    
    log_info "Ejecutando tests del bloque $block en Docker..."
    
    docker run --rm \
        -v "$(pwd):/workspace" \
        c100-ejercicios:latest \
        bash -c "cd /workspace && ./tools/quick_test.sh $block"
}

# Limpiar contenedores e imágenes
clean_docker() {
    log_warning "Limpiando contenedores e imágenes de c100-ejercicios..."
    
    # Detener contenedores
    docker stop c100-dev c100-tcp-server c100-analyzer 2>/dev/null || true
    
    # Eliminar contenedores
    docker rm c100-dev c100-tcp-server c100-analyzer 2>/dev/null || true
    
    # Eliminar imagen
    docker rmi c100-ejercicios:latest 2>/dev/null || true
    
    # Limpiar volúmenes huérfanos
    docker volume prune -f
    
    log_success "Limpieza completada"
}

# Ver logs del contenedor
show_logs() {
    log_info "Mostrando logs del contenedor c100-dev..."
    docker logs -f c100-dev
}

# Abrir shell en contenedor existente
open_shell() {
    log_info "Abriendo shell en contenedor c100-dev..."
    docker exec -it c100-dev /bin/bash
}

# Detener todos los contenedores
stop_containers() {
    log_info "Deteniendo todos los contenedores..."
    docker stop c100-dev c100-tcp-server c100-analyzer 2>/dev/null || true
    log_success "Contenedores detenidos"
}

# Docker Compose up
compose_up() {
    log_info "Iniciando servicios con docker-compose..."
    docker-compose up -d
    log_success "Servicios iniciados"
    log_info "Para conectar al contenedor principal: docker exec -it c100-ejercicios-dev /bin/bash"
}

# Docker Compose down
compose_down() {
    log_info "Deteniendo servicios de docker-compose..."
    docker-compose down
    log_success "Servicios detenidos"
}

# Función principal
main() {
    check_docker
    
    case "${1:-help}" in
        "build")
            build_image
            ;;
        "run")
            build_image
            run_container
            ;;
        "test")
            build_image
            run_tests
            ;;
        "test-block")
            build_image
            run_test_block "$2"
            ;;
        "clean")
            clean_docker
            ;;
        "logs")
            show_logs
            ;;
        "shell")
            open_shell
            ;;
        "stop")
            stop_containers
            ;;
        "compose-up")
            compose_up
            ;;
        "compose-down")
            compose_down
            ;;
        "help"|"--help"|"-h")
            show_help
            ;;
        *)
            log_error "Comando desconocido: $1"
            echo ""
            show_help
            exit 1
            ;;
    esac
}

# Ejecutar función principal con todos los argumentos
main "$@"
