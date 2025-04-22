#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Jugador {
    char nombre[50];
    int puntaje;
};

int leer_jugadores(struct Jugador jugadores[], int *total) {
    FILE *archivo = fopen("jugadores.txt", "r");
    if (archivo == NULL) {
        printf("no se encontro el archivo de jugadores, se creara uno nuevo al guardar\n");
        *total = 0;
        return 0;
    }

    *total = 0;
    while (fscanf(archivo, "Nombre: %[^\n]\n", jugadores[*total].nombre) != EOF) {
        fscanf(archivo, "Puntaje: %d\n", &jugadores[*total].puntaje);
        fscanf(archivo, "\n"); // para la linea en blanco entre registros
        (*total)++;

        if (*total >= 100) {
            printf("se alcanzo el limite de jugadores\n");
            break;
        }
    }

    fclose(archivo);
    return 1;
}

void guardar_jugadores(struct Jugador jugadores[], int total) {
    FILE *archivo = fopen("jugadores.txt", "w");
    if (archivo == NULL) {
        printf("error al guardar los jugadores\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(archivo, "Nombre: %s\n", jugadores[i].nombre);
        fprintf(archivo, "Puntaje: %d\n", jugadores[i].puntaje);
        fprintf(archivo, "\n");
    }

    fclose(archivo);
    printf("jugadores guardados correctamente\n");
}

void ingresar_jugador(struct Jugador jugadores[], int *total) {
    if (*total >= 100) {
        printf("limite de jugadores alcanzado\n");
        return;
    }

    printf("ingrese el nombre del jugador: ");
    scanf(" %[^\n]", jugadores[*total].nombre);
    printf("ingrese el puntaje: ");
    scanf("%d", &jugadores[*total].puntaje);

    (*total)++;
    guardar_jugadores(jugadores, *total);
}

void mostrar_top_10(struct Jugador jugadores[], int total) {
    if (total == 0) {
        printf("no hay jugadores cargados\n");
        return;
    }

    for (int i = 0; i < total - 1; i++) {
        for (int j = i + 1; j < total; j++) {
            if (jugadores[j].puntaje > jugadores[i].puntaje) {
                struct Jugador aux = jugadores[i];
                jugadores[i] = jugadores[j];
                jugadores[j] = aux;
            }
        }
    }

    int mostrar = total < 10 ? total : 10;
    printf("\ntop %d jugadores\n", mostrar);
    for (int i = 0; i < mostrar; i++) {
        printf("nombre: %s\n", jugadores[i].nombre);
        printf("puntaje: %d\n\n", jugadores[i].puntaje);
    }
}

void buscar_jugador(struct Jugador jugadores[], int total) {
    char nombre_buscar[50];
    int encontrado = 0;

    printf("ingrese el nombre del jugador a buscar: ");
    scanf(" %[^\n]", nombre_buscar);

    for (int i = 0; i < total; i++) {
        if (strcmp(jugadores[i].nombre, nombre_buscar) == 0) {
            printf("\njugador encontrado:\n");
            printf("nombre: %s\n", jugadores[i].nombre);
            printf("puntaje: %d\n", jugadores[i].puntaje);
            encontrado = 1;
        }
    }

    if (!encontrado) {
        printf("no se encontro un jugador con ese nombre\n");
    }
}

int main() {
    struct Jugador jugadores[100];
    int total_jugadores = 0;

    leer_jugadores(jugadores, &total_jugadores);

    int opcion;

    do {
        printf("\nmenu:\n");
        printf("1. ingresar jugador\n");
        printf("2. mostrar top 10\n");
        printf("3. buscar jugador por nombre\n");
        printf("0. salir\n");
        printf("opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                ingresar_jugador(jugadores, &total_jugadores);
                break;
            case 2:
                mostrar_top_10(jugadores, total_jugadores);
                break;
            case 3:
                buscar_jugador(jugadores, total_jugadores);
                break;
            case 0:
                printf("saliendo...\n");
                break;
            default:
                printf("opcion no valida\n");
        }
    } while (opcion != 0);

    return 0;
}
