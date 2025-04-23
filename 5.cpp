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
    if (archivo == NULL) return 0;

    int total = 0;
    while (fscanf(archivo, "Nombre: %s\n", personas[total].nombre) != EOF) {
        fscanf(archivo, "Apellido: %s\n", personas[total].apellido);
        fscanf(archivo, "DNI: %d\n", &personas[total].dni);
        fscanf(archivo, "\n");
        total++;
    }

    fclose(archivo);
    return total;
}

void guardar_cuentas(const char *nombre_archivo, struct Persona personas[], int total) {
    FILE *archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("error al guardar en %s\n", nombre_archivo);
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(archivo, "Nombre: %s\n", personas[i].nombre);
        fprintf(archivo, "Apellido: %s\n", personas[i].apellido);
        fprintf(archivo, "DNI: %d\n\n", personas[i].dni);
    }

    fclose(archivo);
}

int buscar_por_dni(struct Persona personas[], int total, int dni) {
    for (int i = 0; i < total; i++) {
        if (personas[i].dni == dni) return i;
    }
    return -1;
}

void mostrar_persona(struct Persona p) {
    printf("nombre: %s\n", p.nombre);
    printf("apellido: %s\n", p.apellido);
    printf("dni: %d\n", p.dni);
}

int comparar_por_dni(const void *a, const void *b) {
    struct Persona *p1 = (struct Persona *)a;
    struct Persona *p2 = (struct Persona *)b;
    return p1->dni - p2->dni;
}

int comparar_por_nombre_apellido(const void *a, const void *b) {
    struct Persona *p1 = (struct Persona *)a;
    struct Persona *p2 = (struct Persona *)b;
    int cmp = strcmp(p1->nombre, p2->nombre);
    if (cmp == 0)
        return strcmp(p1->apellido, p2->apellido);
    return cmp;
}

void mostrar_ordenadas(struct Persona personas[], int total) {
    int criterio;
    printf("ordenar por:\n1. dni\n2. nombre y apellido\nopcion: ");
    scanf("%d", &criterio);

    if (criterio == 1) {
        qsort(personas, total, sizeof(struct Persona), comparar_por_dni);
    } else if (criterio == 2) {
        qsort(personas, total, sizeof(struct Persona), comparar_por_nombre_apellido);
    } else {
        printf("opcion no valida\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        mostrar_persona(personas[i]);
        printf("\n");
    }
}

void modificar_en_copia(struct Persona personas[], int total) {
    int dni, indice;
    printf("ingrese el dni de la persona a modificar: ");
    scanf("%d", &dni);

    indice = buscar_por_dni(personas, total, dni);
    if (indice == -1) {
        printf("persona no encontrada\n");
        return;
    }

    struct Persona copia[100];
    memcpy(copia, personas, sizeof(struct Persona) * total);
    printf("datos actuales en copia:\n");
    mostrar_persona(copia[indice]);

    int opcion;
    printf("que desea modificar?\n1. nombre\n2. apellido\n3. dni\nopcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1:
            printf("nuevo nombre: ");
            scanf("%s", copia[indice].nombre);
            break;
        case 2:
            printf("nuevo apellido: ");
            scanf("%s", copia[indice].apellido);
            break;
        case 3:
            printf("nuevo dni: ");
            scanf("%d", &copia[indice].dni);
            break;
        default:
            printf("opcion no valida\n");
            return;
    }

    guardar_cuentas("copiasCuenta.txt", copia, total);

    printf("datos en copia luego del cambio:\n");
    mostrar_persona(copia[indice]);

    printf("desea aplicar este cambio al archivo original? (1: si, 0: no): ");
    int aplicar;
    scanf("%d", &aplicar);
    if (aplicar == 1) {
        personas[indice] = copia[indice];
        guardar_cuentas("cuentas.txt", personas, total);
        printf("cambio aplicado al archivo original\n");
    } else {
        printf("cambio descartado, solo fue guardado en la copia\n");
    }
}

int ingresar_persona(struct Persona personas[], int *total) {
    if (*total >= 100) {
        printf("limite de personas alcanzado\n");
        return 0;
    }

    int dni;
    char nombre[30], apellido[30];
    printf("nombre: ");
    scanf("%s", nombre);
    printf("apellido: ");
    scanf("%s", apellido);
    printf("dni: ");
    scanf("%d", &dni);

    for (int i = 0; i < *total; i++) {
        if (personas[i].dni == dni) {
            printf("ya existe una persona con ese dni\n");
            return 0;
        }
    }

    strcpy(personas[*total].nombre, nombre);
    strcpy(personas[*total].apellido, apellido);
    personas[*total].dni = dni;

    (*total)++;
    guardar_cuentas("cuentas.txt", personas, *total);
    printf("persona agregada\n");
    return 1;
}

int main() {
    struct Persona personas[100];
    int total = leer_cuentas(personas);
    int opcion;

    do {
        printf("\nmenu:\n");
        printf("1. ingresar persona\n");
        printf("2. buscar por dni\n");
        printf("3. mostrar personas ordenadas\n");
        printf("4. modificar persona en copia\n");
        printf("0. salir\nopcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                ingresar_persona(personas, &total);
                break;
            case 2: {
                int dni;
                printf("ingrese el dni a buscar: ");
                scanf("%d", &dni);
                int i = buscar_por_dni(personas, total, dni);
                if (i != -1) mostrar_persona(personas[i]);
                else printf("no se encontro persona\n");
                break;
            }
            case 3:
                mostrar_ordenadas(personas, total);
                break;
            case 4:
                modificar_en_copia(personas, total);
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
