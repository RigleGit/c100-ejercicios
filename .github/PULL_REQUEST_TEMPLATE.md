# 🚀 Pull Request

## 📋 Descripción
<!-- Describe claramente qué cambia este PR -->

## 🎯 Tipo de Cambio
<!-- Marca con 'x' todos los que apliquen -->
- [ ] 🐛 Bug fix
- [ ] ✨ Nueva funcionalidad
- [ ] 📚 Documentación
- [ ] 🧪 Tests
- [ ] 🔧 Refactoring
- [ ] 🐳 Docker/DevContainer
- [ ] 🛠️ Herramientas/Scripts
- [ ] 💥 Breaking change

## 🔗 Issues Relacionados
<!-- Enlaza los issues que resuelve este PR -->
- Fixes #(issue_number)
- Related to #(issue_number)

## 🧪 Testing
<!-- Describe cómo has probado los cambios -->

### Tests Ejecutados
- [ ] `./tools/test_everything_fresh.sh`
- [ ] `./tools/quick_test.sh [BLOQUE]`
- [ ] `./tools/docker_build.sh test`
- [ ] Tests específicos: \_\_\_\_\_\_\_\_\_\_

### Nuevos Tests
- [ ] He agregado tests para la nueva funcionalidad
- [ ] Los tests existentes siguen pasando
- [ ] He actualizado la documentación de tests

## 📸 Screenshots (si aplica)
<!-- Agrega capturas si los cambios afectan la salida visual -->

## 📚 Documentación
- [ ] He actualizado README.md si es necesario
- [ ] He actualizado DOCKER_README.md si es necesario
- [ ] He actualizado tools/README.md si es necesario
- [ ] He agregado comentarios en el código
- [ ] He actualizado docstrings/headers

## ✅ Checklist de Calidad

### Código
- [ ] El código sigue el estilo del proyecto
- [ ] He ejecutado clang-format si es necesario
- [ ] No hay warnings de compilación
- [ ] He verificado con cppcheck (si aplica)

### Tests
- [ ] Todos los tests existentes pasan
- [ ] He agregado tests para nuevas funciones
- [ ] Los tests cubren casos edge
- [ ] No hay memory leaks (verificado con Valgrind si aplica)

### Docker
- [ ] Los cambios funcionan en Docker
- [ ] DevContainer sigue funcionando
- [ ] Docker Compose sigue funcionando
- [ ] He probado en entorno limpio

### Documentación
- [ ] README está actualizado
- [ ] Comentarios del código están actualizados
- [ ] Ejemplos de uso están actualizados
- [ ] No hay documentación obsoleta

## 🔍 Impacto

### Ejercicios Afectados
<!-- Lista los ejercicios/bloques que cambian -->
- Bloque X: Ejercicios YYY-ZZZ
- Nuevo ejercicio: XXX-nombre

### Breaking Changes
<!-- Si hay cambios que rompen compatibilidad -->
- [ ] No hay breaking changes
- [ ] Breaking changes documentados abajo:

### Performance
- [ ] No impact en performance
- [ ] Mejora performance
- [ ] Puede afectar performance (detallado abajo)

## 📝 Notas Adicionales
<!-- Cualquier información adicional para reviewers -->

## 🎓 Valor Educativo
<!-- Para nuevos ejercicios o cambios educativos -->
- **Conceptos enseñados**: 
- **Nivel de dificultad**: 
- **Prerequisitos**: 
- **Relación con otros ejercicios**: 

---

## 🏷️ Para Reviewers

### Areas de Enfoque
- [ ] Lógica del código
- [ ] Tests y cobertura
- [ ] Documentación
- [ ] Compatibilidad Docker
- [ ] Performance
- [ ] Valor educativo

### Reviewer Checklist
- [ ] Code review completo
- [ ] Tests ejecutados localmente
- [ ] Docker probado
- [ ] Documentación revisada
- [ ] Approved / Request Changes / Comment
