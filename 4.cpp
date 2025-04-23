#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Persona {
    char nombre[30]; 
    char apellido[30];
    int dni;
};

int leer_cuentas(struct Persona personas[]) {
    FILE *archivo = fopen("cuentas.txt", "r");
    if (archivo == NULL) {
        printf("no se encontro el archivo de cuentas, se comienza con una lista vacia\n");
        return 0;
    }

    int total_personas = 0;
    while (fscanf(archivo, "Nombre: %s\n", personas[total_personas].nombre) != EOF) {
        fscanf(archivo, "Apellido: %s\n", personas[total_personas].apellido);
        fscanf(archivo, "DNI: %d\n", &personas[total_personas].dni);
        fscanf(archivo, "\n");
        total_personas++;
        if (total_personas >= 100) {
            printf("se alcanzo el limite de personas\n");
            break;
        }
    }

    fclose(archivo);
    return total_personas;
}

void guardar_cuentas(struct Persona personas[], int total_personas) {
    FILE *archivo = fopen("cuentas.txt", "w");
    if (archivo == NULL) {
        printf("error al abrir el archivo para guardar cuentas\n");
        return;
    }

    for (int i = 0; i < total_personas; i++) {
        fprintf(archivo, "Nombre: %s\n", personas[i].nombre);
        fprintf(archivo, "Apellido: %s\n", personas[i].apellido);
        fprintf(archivo, "DNI: %d\n", personas[i].dni);
        fprintf(archivo, "\n");
    }

    fclose(archivo);
    printf("se guardo correctamente la informacion\n");
}

int ingresar_persona(struct Persona personas[], int *total_personas) {
    if (*total_personas >= 100) {
        printf("no se pueden agregar mas personas, limite alcanzado\n");
        return 0;
    }

    int dni_nuevo;
    char nombre_nuevo[30];
    char apellido_nuevo[30];

    printf("\ningrese el nombre de la persona: ");
    scanf("%s", nombre_nuevo);
    printf("ingrese el apellido de la persona: ");
    scanf("%s", apellido_nuevo);
    printf("ingrese el dni: ");
    scanf("%d", &dni_nuevo);

    for (int i = 0; i < *total_personas; i++) {
        if (personas[i].dni == dni_nuevo) {
            printf("ya existe una persona con ese dni, no se puede agregar\n");
            return 0;
        }
    }

    strcpy(personas[*total_personas].nombre, nombre_nuevo);
    strcpy(personas[*total_personas].apellido, apellido_nuevo);
    personas[*total_personas].dni = dni_nuevo;

    printf("\nse agrego a %s %s con dni %d\n", 
        personas[*total_personas].nombre, 
        personas[*total_personas].apellido, 
        personas[*total_personas].dni);
    
    (*total_personas)++;
    return 1;
}

void buscar_pers_dni(struct Persona personas[], int total_personas, int dni) {
    for (int i = 0; i < total_personas; i++) {
        if (personas[i].dni == dni) {
            printf("\nnombre: %s\n", personas[i].nombre);
            printf("apellido: %s\n", personas[i].apellido);
            printf("dni: %d\n", personas[i].dni);
            return;
        }
    }
    printf("no se encontro un usuario con ese dni\n");
}

void buscar_pers_nombre_apellido(struct Persona personas[], int total_personas, char nombre[], char apellido[]) {
    int encontrado = 0;
    for (int i = 0; i < total_personas; i++) {
        if (strcmp(personas[i].nombre, nombre) == 0 && strcmp(personas[i].apellido, apellido) == 0) {
            printf("\nnombre: %s\n", personas[i].nombre);
            printf("apellido: %s\n", personas[i].apellido);
            printf("dni: %d\n", personas[i].dni);
            encontrado = 1;
        }
    }
    if (!encontrado) {
        printf("no se encontro ningun usuario con ese nombre y apellido\n");
    }
}

int comparar_por_dni(const void *a, const void *b) {
    struct Persona *p1 = (struct Persona *)a;
    struct Persona *p2 = (struct Persona *)b;
    return p1->dni - p2->dni;
}

int comparar_por_nombre_apellido(const void *a, const void *b) {
    struct Persona *p1 = (struct Persona *)a;
    struct Persona *p2 = (struct Persona *)b;
    
    int cmp_nombre = strcmp(p1->nombre, p2->nombre);
    if (cmp_nombre != 0) return cmp_nombre;

    return strcmp(p1->apellido, p2->apellido);
}

void mostrar_ordenadas(struct Persona personas[], int total_personas) {
    if (total_personas == 0) {
        printf("no hay personas cargadas\n");
        return;
    }

    int criterio;
    printf("elegir criterio de orden:\n");
    printf("1. por dni\n");
    printf("2. por nombre y apellido\n");
    printf("opcion: ");
    scanf("%d", &criterio);

    if (criterio == 1) {
        qsort(personas, total_personas, sizeof(struct Persona), comparar_por_dni);
    } else if (criterio == 2) {
        qsort(personas, total_personas, sizeof(struct Persona), comparar_por_nombre_apellido);
    } else {
        printf("criterio no valido\n");
        return;
    }

    printf("\nlistado ordenado:\n");
    for (int i = 0; i < total_personas; i++) {
        printf("nombre: %s\n", personas[i].nombre);
        printf("apellido: %s\n", personas[i].apellido);
        printf("dni: %d\n\n", personas[i].dni);
    }
}

int main() {
    struct Persona personas[100];
    int total_personas = leer_cuentas(personas);

    int opcion, dni_busqueda;
    char nombre_busqueda[30], apellido_busqueda[30];

    do {
        printf("\nopciones:\n");
        printf("1. ingresar persona\n");
        printf("2. buscar persona por dni\n");
        printf("3. buscar persona por nombre y apellido\n");
        printf("4. mostrar personas ordenadas\n");
        printf("0. salir\n");
        printf("elegir una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                if (ingresar_persona(personas, &total_personas)) {
                    guardar_cuentas(personas, total_personas);
                }
                break;
            case 2:
                printf("ingresar el dni a buscar: ");
                scanf("%d", &dni_busqueda);
                buscar_pers_dni(personas, total_personas, dni_busqueda);
                break;
            case 3:
                printf("ingresar el nombre a buscar: ");
                scanf("%s", nombre_busqueda);
                printf("ingresar el apellido a buscar: ");
                scanf("%s", apellido_busqueda);
                buscar_pers_nombre_apellido(personas, total_personas, nombre_busqueda, apellido_busqueda);
                break;
            case 4:
                mostrar_ordenadas(personas, total_personas);
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
