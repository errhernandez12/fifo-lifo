#include <stdio.h>    // Incluye funciones estándar de entrada/salida (como printf)
#include <stdlib.h>   // Incluye gestión de memoria dinámica (malloc, free)
#include <string.h>   // Incluye funciones para manipular textos (strcpy)

// Definimos la estructura 'Registro', que será cada "plato" en nuestra pila
typedef struct comparte {
    char nombre[50];         // Arreglo para el texto del nombre
    char pais[50];      // Arreglo para el texto de la dirección
    int edad;                // Entero para la edad
    struct comparte *next;   // Puntero hacia el elemento de ARRIBA en la pila
    struct comparte *prev;   // Puntero hacia el elemento de ABAJO en la pila
} Registro; 

// Estructura que administrará toda la Pila
typedef struct l {
    Registro *Inicial;       // Centinela que funciona como la "base" de la mesa
    Registro *Final;         // Centinela que funciona como el "tope máximo" imaginario
    int len;                 // Cantidad actual de elementos apilados
} Lista;

// Función para reservar memoria para un nuevo Registro
Registro *crear() {
    Registro *T;                                  // Puntero temporal de tipo Registro
    T = (Registro *)malloc(sizeof(Registro));     // Solicita un bloque de memoria RAM
    T->next = NULL;                               // Limpia la referencia al siguiente nodo
    T->prev = NULL;                               // Limpia la referencia al nodo anterior
    return T;                                     // Devuelve la memoria solicitada
}

// Inicializa la Pila vacía
void crear_lista(Lista *lista) {
    Registro *N_inicial = crear();                // Crea la base
    Registro *N_final = crear();                  // Crea el tope superior
    
    lista->Inicial = N_inicial;                   // Ajusta el apuntador de la estructura a la base
    lista->Final = N_final;                       // Ajusta el apuntador de la estructura al tope
    lista->len = 0;                               // Pila sin elementos reales
    
    lista->Inicial->next = N_final;               // La base apunta hacia el tope directamente
    lista->Inicial->prev = NULL;                  // Debajo de la base no hay nada
    
    lista->Final->prev = N_inicial;               // Debajo del tope está la base
    lista->Final->next = NULL;                    // Arriba del tope no hay nada
}

// Función para APILAR (Push): Pone el elemento hasta arriba
int agregar(Lista *lista, char nombre[50], char pais[50], int edad) {
    Registro *N = crear();                        // Crea el nuevo nodo en RAM
    if(N == NULL) return 0;                       // Previene bloqueos si no hay RAM
        
    strcpy(N->nombre, nombre);                    // Guarda el nombre en el nodo
    strcpy(N->pais, pais);              // Guarda la dirección
    N->edad = edad;                               // Guarda la edad
    
    // Insertamos justo debajo del tope imaginario (Final)
    N->prev = lista->Final->prev;                 // El de 'abajo' del nuevo será el que antes estaba hasta arriba
    N->next = lista->Final;                       // El de 'arriba' del nuevo es el tope imaginario (Final)
    
    lista->Final->prev->next = N;                 // Al nodo que antes estaba hasta arriba, le ponemos este nuevo encima
    lista->Final->prev = N;                       // Al tope imaginario le indicamos que este nuevo nodo está debajo de él
    
    lista->len++;                                 // Incrementa conteo de la pila
    return 1;                                     // Retorna éxito
}

// Función para DESAPILAR (Pop): Extrae el último que llegó (LIFO)
void extraer_lifo(Lista *lista) {
    if(lista->len == 0) {                         // Verifica que existan elementos
        printf("\n[Error] LIFO Vacio: No hay elementos para extraer.\n"); // Reporta vacío
        return;                                   // Aborta la ejecución de esta función
    }
    
    // El 'último' (el que está hasta arriba) está justo antes del centinela Final
    Registro *ultimo = lista->Final->prev;        // Localizamos al que está en el tope de la pila
    
    // Mostramos a quién quitamos
    printf("\n[LIFO] Sacando del tope a:\n");
    printf(" Nombre:    %s\n", ultimo->nombre);
    printf(" Pais: %s\n", ultimo->pais);
    printf(" Edad:      %d\n", ultimo->edad);
    printf("---------------------------------------\n");
    // Puenteamos los punteros para "saltarnos" al elemento que vamos a sacar
    lista->Final->prev = ultimo->prev;            // El tope imaginario ahora se apoya en el penúltimo nodo
    ultimo->prev->next = lista->Final;            // El penúltimo nodo ahora apunta directo al tope imaginario
    
    free(ultimo);                                 // Destruimos el nodo superior liberando la memoria
    lista->len--;                                 // Reducimos el conteo
}

// Bloque principal de ejecución (Programa)
int main() {
    Lista pila_lifo;                              // Declaramos la Pila
    crear_lista(&pila_lifo);                      // Inicializamos
    
    printf("--- APILANDO ELEMENTOS (LIFO) ---\n"); // Mensaje de inicio
    
    // Apilamos en orden (El primero se va al fondo, el último queda arriba)
    agregar(&pila_lifo, "Ernesto Michel", "Cuba", 30); // Queda hasta abajo
    printf("Apilado Ernesto Michel.\n");            // Confirmamos
    
    agregar(&pila_lifo, "Alondra", "Mexico", 22);  // Queda en medio
    printf("Apilada Alondra.\n");                 // Confirmamos
    
    agregar(&pila_lifo, "Braulio", "Cuba", 26); // Queda hasta ARRIBA
    printf("Apilado Braulio.\n");                 // Confirmamos
    
    printf("\n--- DESAPILANDO ELEMENTOS (LIFO) ---\n"); // Mensaje de extracción
    
    // Como es LIFO, el último en entrar DEBE ser el primero en salir (Braulio)
    extraer_lifo(&pila_lifo);                     // Sale Braulio (estaba hasta arriba)
    extraer_lifo(&pila_lifo);                     // Sale Alondra (quedó arriba)
    extraer_lifo(&pila_lifo);                     // Sale Jose Ernesto (el que estaba en el fondo)
    
    extraer_lifo(&pila_lifo);                     // Intentará extraer, pero mostrará error "LIFO vacío"
    
    return 0;                                     // Fin exitoso
}
