#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "estructura_persona.h"

// Crear persona con valores dados
Persona crear_persona(const char *nombre, int edad) {
    Persona p;
    
    // Copiar nombre de forma segura
    strncpy(p.nombre, nombre, MAX_NOMBRE - 1);
    p.nombre[MAX_NOMBRE - 1] = '\0';  // Asegurar terminación
    
    p.edad = edad;
    
    return p;
}

// Leer persona desde entrada del usuario
Persona leer_persona_usuario(void) {
    Persona p;
    
    printf("Introduce el nombre: ");
    fgets(p.nombre, MAX_NOMBRE, stdin);
    
    // Eliminar salto de línea
    p.nombre[strcspn(p.nombre, "\n")] = '\0';
    
    printf("Introduce la edad: ");
    scanf("%d", &p.edad);
    getchar(); // Consumir el salto de línea restante
    
    return p;
}

// Mostrar persona en formato simple
void mostrar_persona(const Persona *p) {
    if (p == NULL) {
        printf("Persona inválida (NULL)\n");
        return;
    }
    
    printf("Nombre: %s, Edad: %d\n", p->nombre, p->edad);
}

// Mostrar persona en formato simple (una línea)
void mostrar_persona_simple(const Persona *p) {
    if (p == NULL) {
        printf("(Persona inválida)\n");
        return;
    }
    
    printf("%s (%d años)\n", p->nombre, p->edad);
}

// Mostrar persona en formato detallado
void mostrar_persona_detallada(const Persona *p) {
    if (p == NULL) {
        printf("Error: Persona inválida (puntero NULL)\n");
        return;
    }
    
    printf("╔══════════════════════════════════════╗\n");
    printf("║            DATOS PERSONALES          ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║ Nombre: %-28s ║\n", p->nombre);
    printf("║ Edad:   %-28d ║\n", p->edad);
    
    // Información adicional basada en edad
    printf("║ Categoría: %-25s ║\n", obtener_categoria_edad(p->edad));
    printf("║ Estado: %-27s ║\n", validar_persona(p) ? "Válido" : "Inválido");
    
    printf("╚══════════════════════════════════════╝\n");
}

// Validar datos de una persona
bool validar_persona(const Persona *p) {
    if (p == NULL) {
        return false;
    }
    
    // Verificar que el nombre no esté vacío
    if (strlen(p->nombre) == 0) {
        return false;
    }
    
    // Verificar que la edad sea válida (0-150)
    if (p->edad < 0 || p->edad > 150) {
        return false;
    }
    
    return true;
}

// Comparar si dos personas tienen la misma edad
bool misma_edad(const Persona *p1, const Persona *p2) {
    if (p1 == NULL || p2 == NULL) {
        return false;
    }
    
    return p1->edad == p2->edad;
}

// Obtener la persona de mayor edad entre dos
Persona* persona_mayor(Persona *p1, Persona *p2) {
    if (p1 == NULL && p2 == NULL) {
        return NULL;
    }
    
    if (p1 == NULL) return p2;
    if (p2 == NULL) return p1;
    
    return (p1->edad >= p2->edad) ? p1 : p2;
}

// Copiar datos de una persona a otra
void copiar_persona(Persona *destino, const Persona *origen) {
    if (destino == NULL || origen == NULL) {
        return;
    }
    
    strcpy(destino->nombre, origen->nombre);
    destino->edad = origen->edad;
}

// Obtener categoría de edad (función auxiliar)
const char* obtener_categoria_edad(int edad) {
    if (edad < 0) return "Inválida";
    if (edad < 3) return "Bebé";
    if (edad < 12) return "Niño/a";
    if (edad < 18) return "Adolescente";
    if (edad < 30) return "Joven";
    if (edad < 60) return "Adulto";
    if (edad < 80) return "Adulto mayor";
    return "Anciano/a";
}

// Funciones para trabajar con arrays de personas

// Mostrar lista de personas
void mostrar_lista_personas(const Persona personas[], int cantidad) {
    if (personas == NULL || cantidad <= 0) {
        printf("Lista vacía o inválida\n");
        return;
    }
    
    printf("Lista de %d persona(s):\n", cantidad);
    for (int i = 0; i < cantidad; i++) {
        printf("%2d. ", i + 1);
        mostrar_persona_simple(&personas[i]);
    }
}

// Encontrar la persona de mayor edad en un array
Persona* encontrar_mayor(Persona personas[], int cantidad) {
    if (personas == NULL || cantidad <= 0) {
        return NULL;
    }
    
    Persona *mayor = &personas[0];
    
    for (int i = 1; i < cantidad; i++) {
        if (personas[i].edad > mayor->edad) {
            mayor = &personas[i];
        }
    }
    
    return mayor;
}

// Encontrar la persona de menor edad en un array
Persona* encontrar_menor(Persona personas[], int cantidad) {
    if (personas == NULL || cantidad <= 0) {
        return NULL;
    }
    
    Persona *menor = &personas[0];
    
    for (int i = 1; i < cantidad; i++) {
        if (personas[i].edad < menor->edad) {
            menor = &personas[i];
        }
    }
    
    return menor;
}

// Calcular edad promedio
double edad_promedio(const Persona personas[], int cantidad) {
    if (personas == NULL || cantidad <= 0) {
        return 0.0;
    }
    
    int suma = 0;
    for (int i = 0; i < cantidad; i++) {
        suma += personas[i].edad;
    }
    
    return (double)suma / cantidad;
}

// Buscar persona por nombre
int buscar_por_nombre(const Persona personas[], int cantidad, const char *nombre) {
    if (personas == NULL || cantidad <= 0 || nombre == NULL) {
        return -1;
    }
    
    for (int i = 0; i < cantidad; i++) {
        if (strcmp(personas[i].nombre, nombre) == 0) {
            return i;
        }
    }
    
    return -1;
}

// Buscar persona por nombre (insensible a mayúsculas)
int buscar_por_nombre_insensible(const Persona personas[], int cantidad, const char *nombre) {
    if (personas == NULL || cantidad <= 0 || nombre == NULL) {
        return -1;
    }
    
    for (int i = 0; i < cantidad; i++) {
        if (strcasecmp(personas[i].nombre, nombre) == 0) {
            return i;
        }
    }
    
    return -1;
}

// Ordenar array de personas por edad (burbuja)
void ordenar_por_edad(Persona personas[], int cantidad) {
    if (personas == NULL || cantidad <= 1) {
        return;
    }
    
    for (int i = 0; i < cantidad - 1; i++) {
        bool intercambio = false;
        
        for (int j = 0; j < cantidad - 1 - i; j++) {
            if (personas[j].edad > personas[j + 1].edad) {
                // Intercambiar personas
                Persona temp = personas[j];
                personas[j] = personas[j + 1];
                personas[j + 1] = temp;
                intercambio = true;
            }
        }
        
        // Si no hubo intercambios, ya está ordenado
        if (!intercambio) {
            break;
        }
    }
}

// Ordenar array de personas por nombre
void ordenar_por_nombre(Persona personas[], int cantidad) {
    if (personas == NULL || cantidad <= 1) {
        return;
    }
    
    for (int i = 0; i < cantidad - 1; i++) {
        bool intercambio = false;
        
        for (int j = 0; j < cantidad - 1 - i; j++) {
            if (strcmp(personas[j].nombre, personas[j + 1].nombre) > 0) {
                // Intercambiar personas
                Persona temp = personas[j];
                personas[j] = personas[j + 1];
                personas[j + 1] = temp;
                intercambio = true;
            }
        }
        
        if (!intercambio) {
            break;
        }
    }
}

// Copiar array de personas
void copiar_array_personas(Persona destino[], const Persona origen[], int cantidad) {
    if (destino == NULL || origen == NULL || cantidad <= 0) {
        return;
    }
    
    for (int i = 0; i < cantidad; i++) {
        copiar_persona(&destino[i], &origen[i]);
    }
}

// Contar personas en rango de edad
int contar_en_rango_edad(const Persona personas[], int cantidad, int edad_min, int edad_max) {
    if (personas == NULL || cantidad <= 0) {
        return 0;
    }
    
    int contador = 0;
    for (int i = 0; i < cantidad; i++) {
        if (personas[i].edad >= edad_min && personas[i].edad <= edad_max) {
            contador++;
        }
    }
    
    return contador;
}

// Verificar si todas las personas son válidas
bool todas_validas(const Persona personas[], int cantidad) {
    if (personas == NULL || cantidad <= 0) {
        return false;
    }
    
    for (int i = 0; i < cantidad; i++) {
        if (!validar_persona(&personas[i])) {
            return false;
        }
    }
    
    return true;
}

// Función auxiliar para strcasecmp en sistemas que no la tienen
#ifndef HAVE_STRCASECMP
int strcasecmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        char c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
        char c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;
        
        if (c1 != c2) {
            return c1 - c2;
        }
        
        s1++;
        s2++;
    }
    
    char c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
    char c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;
    
    return c1 - c2;
}
#endif
