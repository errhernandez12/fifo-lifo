#include <stdio.h>    // Incluye funciones estándar de entrada/salida (como printf)
#include <stdlib.h>   // Incluye gestión de memoria dinámica (malloc, free)
#include <string.h>   // Incluye funciones para manipular textos (strcpy)

// Definimos la estructura 'Registro', que representará a cada persona formada en la fila
typedef struct comparte {
    char nombre[50];         // Arreglo para guardar el nombre de la persona
    char pais[50];      // Arreglo para guardar la dirección de la persona
    int edad;                // Variable entera para guardar la edad
    struct comparte *next;   // Puntero que enlaza hacia la persona de ATRÁS en la fila
    struct comparte *prev;   // Puntero que enlaza hacia la persona de ADELANTE en la fila
} Registro; 

// Definimos la estructura 'Lista' que administrará toda la fila
typedef struct l {
    Registro *Inicial;       // Puntero al inicio ficticio (centinela) de la fila
    Registro *Final;         // Puntero al final ficticio (centinela) de la fila
    int len;                 // Contador para saber cuántas personas reales hay formadas
} Lista;

// Función para reservar memoria en el sistema para un nuevo Registro
Registro *crear() {
    Registro *T;                                  // Declaramos un puntero de tipo Registro
    T = (Registro *)malloc(sizeof(Registro));     // Solicitamos al sistema operativo el tamaño en bytes de un Registro
    T->next = NULL;                               // Inicializamos el enlace 'siguiente' como vacío (NULL)
    T->prev = NULL;                               // Inicializamos el enlace 'anterior' como vacío (NULL)
    return T;                                     // Devolvemos el espacio de memoria listo para usarse
}

// Función para crear la estructura base de la Fila (con sus centinelas)
void crear_lista(Lista *lista) {
    Registro *N_inicial;                          // Declaramos un nodo centinela para el inicio
    Registro *N_final;                            // Declaramos un nodo centinela para el final
    
    N_inicial = crear();                          // Asignamos memoria para el centinela inicial
    N_final = crear();                            // Asignamos memoria para el centinela final
    
    lista->Inicial = N_inicial;                   // Vinculamos el inicio de la lista a N_inicial
    lista->Final = N_final;                       // Vinculamos el final de la lista a N_final
    lista->len = 0;                               // La fila nace con 0 personas reales formadas
    
    lista->Inicial->next = N_final;               // Como está vacía, el siguiente del Inicio es el Final
    lista->Inicial->prev = NULL;                  // Detrás del Inicio no hay nadie
    
    lista->Final->prev = N_inicial;               // Como está vacía, el anterior del Final es el Inicio
    lista->Final->next = NULL;                    // Después del Final no hay nadie
}

// Función para ENCOLAR: Formar a una nueva persona al final de la fila
int agregar(Lista *lista, char nombre[50], char pais[50], int edad) {
    Registro *N;                                  // Declaramos a la nueva persona
    
    N = crear();                                  // Pedimos memoria para la nueva persona
    if(N == NULL)                                 // Verificamos que la memoria se asignó correctamente
        return 0;                                 // Si falló la memoria, abortamos y retornamos 0
        
    strcpy(N->nombre, nombre);                    // Copiamos el texto del parámetro 'nombre' al nodo
    strcpy(N->pais, pais);              // Copiamos el texto del parámetro 'pais' al nodo
    N->edad = edad;                               // Guardamos la edad en el nodo
    
    // Inserción antes del nodo 'Final' (Se forma al final de la fila)
    N->prev = lista->Final->prev;                 // El de 'adelante' del nuevo será el que antes era el último
    N->next = lista->Final;                       // El de 'atrás' del nuevo será la puerta de salida (Final)
    
    lista->Final->prev->next = N;                 // Al que antes era el último, le decimos que su de 'atrás' ahora es el Nuevo
    lista->Final->prev = N;                       // A la puerta de salida (Final) le decimos que su de 'adelante' ahora es el Nuevo
    
    lista->len++;                                 // Aumentamos el contador de personas formadas
    return 1;                                     // Retornamos 1 (éxito)
}

// Función para DESENCOLAR: Atender y sacar a la primera persona que llegó
void extraer_fifo(Lista *lista) {
    if(lista->len == 0) {                         // Verificamos si la fila ya está vacía
        printf("\n[Error] FIFO Vacio: No hay elementos para extraer.\n"); // Avisamos que no hay nadie
        return;                                   // Salimos de la función sin hacer nada
    }
    
    // El 'primero' real está inmediatamente después del centinela Inicial
    Registro *primero = lista->Inicial->next;     // Localizamos a la primera persona formada
    
    // Mostramos sus datos
    printf("\n[FIFO] Atendiendo y sacando a:\n");
    printf(" Nombre:    %s\n", primero->nombre);
    printf(" País: %s\n", primero->pais);
    printf(" Edad:      %d\n", primero->edad);
    printf("---------------------------------------\n");
    
    // Desconectamos al 'primero' de la fila uniendo al Inicio con el Segundo
    lista->Inicial->next = primero->next;         // El Inicio ahora apunta a la segunda persona
    primero->next->prev = lista->Inicial;         // La segunda persona ahora reconoce al Inicio como su 'adelante'
    
    free(primero);                                // Liberamos la memoria de la persona atendida (desaparece de la RAM)
    lista->len--;                                 // Disminuimos el contador de personas formadas
}

// Bloque principal de ejecución (Programa)
int main() {
    Lista fila_fifo;                              // Declaramos la variable de nuestra Fila
    crear_lista(&fila_fifo);                      // Inicializamos la estructura base de la Fila
    
    printf("--- FORMANDO PERSONAS EN LA FILA (FIFO) ---\n"); // Título informativo
    
    // Simulamos personas llegando en orden
    agregar(&fila_fifo, "Ernesto Michel", "Cuba", 30); // Llega primero
    printf("Llego Jose Ernesto.\n");              // Confirmamos
    
    agregar(&fila_fifo, "Alondra", "Mexico", 22);  // Llega segunda
    printf("Llego Alondra.\n");                   // Confirmamos
    
    agregar(&fila_fifo, "Gabriel", "Cuba", 24); // Llega tercero
    printf("Llego Braulio.\n");                   // Confirmamos
    
    printf("\n--- ATENDIENDO LA FILA (FIFO) ---\n"); // Título informativo
    
    // Como es FIFO, el primero en salir DEBE ser el primero en entrar (Ernesto Michel)
    extraer_fifo(&fila_fifo);                     // Extrae a Ernesto Michel
    extraer_fifo(&fila_fifo);                     // Extrae a Alondra
    extraer_fifo(&fila_fifo);                     // Extrae a Gabriel
    
    extraer_fifo(&fila_fifo);                     // Intentará extraer, pero mostrará error de "FIFO vacío"
    
    return 0;                                     // Finaliza la ejecución del programa sin errores
}
