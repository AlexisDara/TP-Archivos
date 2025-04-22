#include<stdio.h>
#include<string.h>

struct Persona {
    char nombre[30];
    char apellido[30];
    int dni;
};


int leer_cuentas(struct Persona personas[]) {
    FILE *archivo = fopen("cuentas.txt", "r");
    if (archivo == NULL) {
        printf("No se encontro el archivo de cuentas. Comenzando con una lista vacia.\n");
        return 0;
    }

    int total_personas = 0;
    while (fscanf(archivo, "Nombre: %s\n", personas[total_personas].nombre) != EOF) {
        fscanf(archivo, "Apellido: %s\n", personas[total_personas].apellido);
        fscanf(archivo, "DNI: %d\n", &personas[total_personas].dni);
        fscanf(archivo, "\n");
        total_personas++;
        if (total_personas >= 100) { 
            printf("Se alcanzo el limite de personas.\n");
            break;
        }
    }

    fclose(archivo);
    return total_personas;
}


void guardar_cuentas(struct Persona personas[], int total_personas) {
    FILE *archivo = fopen("cuentas.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo para guardar cuentas.\n");
        return;
    }

    for (int i = 0; i < total_personas; i++) {
        fprintf(archivo, "Nombre: %s\n", personas[i].nombre);
        fprintf(archivo, "Apellido: %s\n", personas[i].apellido);
        fprintf(archivo, "DNI: %d\n", personas[i].dni);
        fprintf(archivo, "\n");
    }

    fclose(archivo);
    printf("Se guardo correctamente la informacion.\n");
}


void ingresar_persona(struct Persona personas[], int *total_personas) {
    if (*total_personas >= 100) {
        printf("No se pueden agregar mas personas. Limite alcanzado.\n");
        return;
    }

    printf("\nIngrese el nombre de la persona: ");
    scanf("%s", personas[*total_personas].nombre);
    printf("Ingrese el apellido de la persona: ");
    scanf("%s", personas[*total_personas].apellido);
    printf("Ingrese el DNI: ");
    scanf("%d", &personas[*total_personas].dni);

    printf("\nSe agrego a %s con DNI %d.\n", personas[*total_personas].nombre, personas[*total_personas].dni);
    (*total_personas)++;
}


void buscar_pers(struct Persona personas[], int total_personas, int dni) {
    for (int i = 0; i < total_personas; i++) {
        if (personas[i].dni == dni) {
            printf("\nNombre: %s\n", personas[i].nombre);
            printf("Apellido: %s\n", personas[i].apellido);
            printf("DNI: %d\n", personas[i].dni);
            return;
        }
    }
    printf("No se encontro un usuario con ese DNI.\n");
}

int main() {
    struct Persona personas[100];
    int total_personas = leer_cuentas(personas);

    int opcion, dni_busqueda;

    do {
        printf("\nOpciones:\n");
        printf("1. Ingresar persona\n");
        printf("2. Buscar persona por DNI\n");
        printf("0. Salir\n");
        printf("Elija una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                ingresar_persona(personas, &total_personas);
                guardar_cuentas(personas, total_personas);
                break;
            case 2:
                printf("Ingrese el DNI a buscar: ");
                scanf("%d", &dni_busqueda);
                buscar_pers(personas, total_personas, dni_busqueda);
                break;
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion no valida.\n");
        }
    } while (opcion != 0);

    return 0;
}

