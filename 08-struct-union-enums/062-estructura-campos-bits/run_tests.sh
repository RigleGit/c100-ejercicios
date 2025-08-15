#!/bin/bash

# Script automático para el ejercicio 062 - Estructura con Bit Fields

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== Ejercicio 062: Estructura con Bit Fields ===${NC}"
echo

# Compilar programa principal
echo -e "${YELLOW}Compilando programa principal...${NC}"
gcc -I./include -Wall -Wextra -g -o bitfields_estado src/bitfields_estado.c
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Compilación exitosa${NC}"
else
    echo -e "${RED}❌ Error en compilación${NC}"
    exit 1
fi

# Compilar tests
echo -e "${YELLOW}Compilando tests...${NC}"
gcc -I. -DEXCLUDE_MAIN -Wall -Wextra -g -o test_simple test_simple.c src/bitfields_estado.c
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Tests compilados correctamente${NC}"
else
    echo -e "${RED}❌ Error compilando tests${NC}"
    exit 1
fi

echo

# Ejecutar tests
echo -e "${BLUE}=== Ejecutando Tests ===${NC}"
./test_simple
test_result=$?

echo

# Ejecutar programa principal si los tests pasan
if [ $test_result -eq 0 ]; then
    echo -e "${BLUE}=== Ejecutando Programa Principal ===${NC}"
    ./bitfields_estado
    echo
    echo -e "${GREEN}✅ Ejercicio 062 completado exitosamente${NC}"
else
    echo -e "${RED}❌ Los tests fallaron. Revisar implementación${NC}"
    exit 1
fi
