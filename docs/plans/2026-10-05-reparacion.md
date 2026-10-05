# Reparación del repositorio de ejercicios: plan de implementación

> Ejecución autorizada por el usuario en este repositorio, por bloques y con commits locales. No hacer push.

**Objetivo:** corregir los fallos de la auditoría del 5 de octubre y entregar comandos reproducibles de compilación y pruebas para los 100 ejercicios.

**Arquitectura:** conservar los programas y APIs útiles. Reparar el lanzador para ejecutar CTest y fallar ante ausencia de pruebas. Sustituir referencias a tests inexistentes por pruebas reales de contratos públicos, sin depender de Criterion para las comprobaciones básicas. Compartir únicamente la configuración repetida necesaria.

**Herramientas:** C99, CMake, CTest, compiladores GCC/Clang, shell y herramientas de comprobación locales; Docker para Linux.

## Bloques y validación

1. Lanzador: prueba con CMake real para éxito, fallo, ausencia de tests, selección inexistente y timeout; reparar propagación de errores y selección; commit.
2. Agenda: reproducir importación destructiva, CSV con comillas, escritura fallida y EOF; corregir persistencia y entrada; validar controles normales; commit.
3. Aritmética 022 y 025: comprobar límites y dominio, corregir overflow; validar con UBSan y regresiones; commits por ejercicio.
4. Portabilidad 061/063/064/070 y contador 076: reproducir build y línea larga, corregir inclusión/contadores, validar con GCC/Clang; commits acotados.
5. Demostraciones 088/091/092: leer API y programas completos, alinear firmas y objetivos, verificar compilación y comportamiento local; commits.
6. Pruebas de los 100 ejercicios: recuperar cobertura de resultados reales y contratos, quitar referencias a archivos inexistentes, ejecutar CMake/CTest por bloques en Linux y macOS. No contar demos sin aserciones como pruebas.
7. Correspondencia: renumerar 085–087 según el libro, elegir 094 canónico conservando material útil como alternativa documentada; comprobar enlaces y commits.
8. Documentación: título actual, comandos verificados, tabla y erratas, afirmaciones de cobertura respaldadas por resultados. Commit.
9. Comprobación final desde árboles limpios: ejecutar suite completa en Linux/macOS, comprobar sanitizadores pertinentes y diff. Registrar resultados y lista de commits locales. No publicar.

Cada reparación funcional sigue reproducción → cambio → prueba → revisión del diff → commit. No se promete una revisión manual exhaustiva de toda función ni ausencia absoluta de bugs.
