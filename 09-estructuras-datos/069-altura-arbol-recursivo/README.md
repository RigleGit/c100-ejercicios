# Ejercicio 069: Altura de Árbol Binario Recursivo

## Enunciado

- Dominar el concepto de **recursión** en árboles binarios
- Entender la definición matemática de **altura** de un árbol
- Implementar algoritmos recursivos para análisis estructural
- Comparar enfoques recursivos vs iterativos
- Analizar la complejidad de algoritmos recursivos
- Manejar casos base y casos recursivos correctamente
- Optimizar operaciones en estructuras jerárquicas

## Comportamiento y límites

La inserción actualiza el tamaño después de crear el nodo. Los duplicados devuelven false sin modificar el tamaño. Aquí un árbol vacío tiene altura 0 y una hoja altura 1; el 068 usa otra convención.

## Compilar y ejecutar

Desde esta carpeta:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/altura_arbol
```

## Pruebas de esta revisión

Inserciones, duplicados, tamaño y altura.

Código: [tests/test_contrato.c](tests/test_contrato.c). CTest ejecuta las pruebas registradas; el lanzador considera un fallo su ausencia.

La solución está en [src/](src/). Consulta el [índice](../../docs/EJERCICIOS.md) para continuar con el libro.
