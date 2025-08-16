#!/bin/bash
# Script para corregir todas las referencias en el README de una vez

# 1. Primero añadir la sección del libro
sed -i '' '/^Un conjunto completo de/a\
\
## 📖 Sobre el Libro\
\
Este repositorio es un **extra del libro "El Lenguaje C en 100 ejercicios explicados"**. Si este código te ha sido de ayuda y quieres apoyar el proyecto o acceder al contenido completo explicado paso a paso, puedes encontrar el libro aquí:\
\
**📘 [El Lenguaje C en 100 ejercicios explicados](https://www.amazon.com/Rodrigo-Iglesias-ebook/dp/B0CRVJKX5H)**\
\
¡Tu apoyo ayuda a mantener este proyecto actualizado y crear más recursos educativos! 🙏\
' README.md

# 2. Actualizar la tabla de contenidos
sed -i '' 's/- \[🎯 Descripción\]/- [📖 Sobre el Libro](#-sobre-el-libro)\
- [🎯 Descripción]/' README.md

# 3. Cambiar todas las referencias del script
sed -i '' 's/test_everything_fresh\.sh/prueba_everything_fresh.sh/g' README.md

echo "README.md corregido exitosamente"
