#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "estructuras.h"
#include "crud.h"
#include "filemanager.h"

// ==========================================
// --- ABM Y LÓGICA DE PLATOS (PRODUCTOS) ---
// ==========================================

void menuAltaProducto(int id_rest) {
    Producto p;
    int auxENTERO = 0;
    char auxCARACTER[150];
    float auxFLOAT = 0.0;
    int invalido = 0;

    printf("\n--- ALTA DE PLATO ---\n");
    do {
        printf("ID del nuevo Plato: ");
        scanf("%d", &auxENTERO);
        getchar();
        invalido = buscarProductoPorID(id_rest, auxENTERO) == 1;
        if(invalido) puts("Ese ID de plato ya existe en este restaurante!");
    } while(invalido);

    p.id_producto = auxENTERO;
    p.id_usuario_restaurante = id_rest;

    do {
        printf("Nombre del Plato: ");
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
        auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = validarNombre(auxCARACTER) == 0;
        if(invalido) puts("Nombre invalido, intente de nuevo");
    } while(invalido);
    strcpy(p.nombre, auxCARACTER);

    do {
        printf("Descripcion del Plato: ");
        fgets(p.descripcion, sizeof(p.descripcion), stdin);
        p.descripcion[strcspn(p.descripcion, "\n")] = '\0';
        invalido = strlen(p.descripcion) == 0;
        if(invalido) puts("Debe llenar la descripcion");
    } while(invalido);

    do {
        printf("Precio: ");
        scanf("%f", &auxFLOAT);
        getchar();
        invalido = (auxFLOAT <= 0);
        if(invalido) puts("El precio debe ser mayor a 0");
    } while(invalido);
    p.precio = auxFLOAT;

    p.estado = 1; // Disponible
    p.activo = 1; // Activo

    if(guardarProducto(p) == 1) {
        printf("¡Plato guardado exitosamente en el menu!\n");
    } else {
        printf("Error: No se pudo guardar el plato.\n");
    }
}

void menuModificarProducto(int id_rest) {
    int auxENTERO = 0;
    char nuevoNombre[50];
    float auxFLOAT = 0.0;
    int invalido = 0;

    printf("\n--- MODIFICAR PLATO ---\n");
    printf("Ingrese el ID del plato a modificar: ");
    scanf("%d", &auxENTERO);
    getchar();

    if(buscarProductoPorID(id_rest, auxENTERO) == 0) {
        puts("Error: El plato no existe en este restaurante.");
    } else {
        do {
            printf("Nuevo Nombre del Plato: ");
            fgets(nuevoNombre, sizeof(nuevoNombre), stdin);
            nuevoNombre[strcspn(nuevoNombre, "\n")] = '\0';
            invalido = validarNombre(nuevoNombre) == 0;
            if(invalido) puts("Nombre invalido");
        } while(invalido);

        do {
            printf("Nuevo Precio: ");
            scanf("%f", &auxFLOAT);
            getchar();
            invalido = (auxFLOAT <= 0);
            if(invalido) puts("El precio debe ser mayor a 0");
        } while(invalido);

        if(modificarProducto(id_rest, auxENTERO, nuevoNombre, auxFLOAT) == 1) {
            printf("¡Plato modificado con exito!\n");
        } else {
            printf("Error al intentar modificar.\n");
        }
    }
}

void menuBajaProducto(int id_rest) {
    int auxENTERO = 0;

    printf("\n--- BAJA DE PLATO ---\n");
    printf("Ingrese el ID del plato a dar de baja: ");
    scanf("%d", &auxENTERO);
    getchar();

    if(buscarProductoPorID(id_rest, auxENTERO) == 0) {
        puts("Error: El plato no existe en este restaurante.");
    } else {
        if(bajaProducto(id_rest, auxENTERO) == 1) {
            printf("¡Plato dado de baja del menu exitosamente!\n");
        } else {
            printf("Error al intentar dar de baja.\n");
        }
    }
}

void gestionPlatos() {
    int id_rest, opcABM;

    printf("\n--- GESTION DE PLATOS (PRODUCTOS) ---\n");
    printf("Ingrese el ID del Restaurante due%co del menu: ", 164);
    scanf("%d", &id_rest);
    getchar();

    // Validamos primero que el restaurante exista y este activo
    if(buscarRestaurantePorID(id_rest) == 0) {
        puts("Error: El restaurante no existe o esta de baja.");
        return;
    }

    do {
        printf("\n--- MENU DEL RESTAURANTE #%d ---\n", id_rest);
        printf("1. Alta de Plato\n");
        printf("2. Modificar Plato\n");
        printf("3. Baja de Plato\n");
        printf("0. Volver al menu principal\n");
        printf("Opcion ABM: ");
        scanf("%d", &opcABM);
        getchar();

        switch(opcABM) {
            case 1:
                menuAltaProducto(id_rest);
                break;
            case 2:
                menuModificarProducto(id_rest);
                break;
            case 3:
                menuBajaProducto(id_rest);
                break;
            case 0:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while(opcABM != 0);
}

int buscarProductoPorID(int id_restaurante, int id_producto) {
    FILE *f = fopen("productos.dat", "rb");
    if(f == NULL) return 0;

    Producto aux;
    while(leerRegistro(f, &aux, sizeof(Producto))) {
        if(aux.id_usuario_restaurante == id_restaurante && aux.id_producto == id_producto && aux.activo == 1) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int guardarProducto(Producto p) {
    FILE *f = abrirArchivo("productos.dat", "ab");
    if(f == NULL) return 0;

    int resultado = escribirRegistro(f, &p, sizeof(Producto));
    fclose(f);
    return resultado;
}

int modificarProducto(int id_restaurante, int id_producto, char nuevoNombre[], float nuevoPrecio) {
    FILE *f = abrirArchivo("productos.dat", "rb+");
    if(f == NULL) return 0;

    Producto aux;
    int modificado = 0;

    while(leerRegistro(f, &aux, sizeof(Producto))) {
        if(aux.id_usuario_restaurante == id_restaurante && aux.id_producto == id_producto && aux.activo == 1) {
            strcpy(aux.nombre, nuevoNombre);
            aux.precio = nuevoPrecio;

            fseek(f, -sizeof(Producto), SEEK_CUR);
            modificado = escribirRegistro(f, &aux, sizeof(Producto));
            break;
        }
    }
    fclose(f);
    return modificado;
}

int bajaProducto(int id_restaurante, int id_producto) {
    FILE *f = abrirArchivo("productos.dat", "rb+");
    if(f == NULL) return 0;

    Producto aux;
    int dadoDeBaja = 0;

    while(leerRegistro(f, &aux, sizeof(Producto))) {
        if(aux.id_usuario_restaurante == id_restaurante && aux.id_producto == id_producto && aux.activo == 1) {
            aux.activo = 0;

            fseek(f, -sizeof(Producto), SEEK_CUR);
            dadoDeBaja = escribirRegistro(f, &aux, sizeof(Producto));
            break;
        }
    }
    fclose(f);
    return dadoDeBaja;
}

// ==========================================
// --- ABM Y LÓGICA DE RESTAURANTES ---
// ==========================================

void pantallaAltaRestaurante()
{
    Restaurante r;
    int auxENTERO = 0;
    char auxCARACTER[150];
    int invalido = 0;
    printf("\n--- ALTA DE RESTAURANTE ---\n");
    do
    {
        printf("ID Restaurante: ");
        scanf("%d", &auxENTERO);
        getchar();
        invalido = buscarRestaurantePorID(auxENTERO) == 1;
        if(invalido) puts("Ese ID ya existe, ingrese otro!");
    }
    while(invalido);
    r.id_restaurante = auxENTERO;

    do
    {
        printf("Nombre del Restaurante: ");
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
        auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = validarNombre(auxCARACTER) == 0;
        if(invalido) puts("Nombre invalido, intente de nuevo");
    }
    while(invalido);
    strcpy(r.nombre, auxCARACTER);

    do
    {
        printf("Descripcion / Rubro: ");
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
        auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = strlen(auxCARACTER) == 0;
        if(invalido) puts("Debe llenar el campo");
    }
    while(invalido);
    strcpy(r.descripcion, auxCARACTER);

    r.activo = 1;

    if(guardarRestaurante(r) == 1)
    {
        printf("¡Restaurante registrado exitosamente!\n");
    }
    else
    {
        printf("Error: No se pudo guardar.\n");
    }
}

void pantallaModificacionRestaurante()
{
    int auxENTERO = 0;
    int invalido = 0;
    char nuevoNombre[50], nuevaDesc[150];
    printf("\n--- MODIFICAR RESTAURANTE ---\n");
    printf("Ingrese el ID del restaurante a modificar: ");
    scanf("%d", &auxENTERO);
    getchar();

    if(buscarRestaurantePorID(auxENTERO) == 0)
    {
        puts("Error: El ID no existe o esta de baja.");
    }
    else
    {
        do
        {
            printf("Nuevo Nombre: ");
            fgets(nuevoNombre, sizeof(nuevoNombre), stdin);
            nuevoNombre[strcspn(nuevoNombre, "\n")] = '\0';
            invalido = validarNombre(nuevoNombre) == 0;
            if(invalido) puts("Nombre invalido");
        }
        while(invalido);

        do
        {
            printf("Nueva Descripcion: ");
            fgets(nuevaDesc, sizeof(nuevaDesc), stdin);
            nuevaDesc[strcspn(nuevaDesc, "\n")] = '\0';
            invalido = strlen(nuevaDesc) == 0;
            if(invalido) puts("Debe llenar el campo");
        }
        while(invalido);

        if(modificarRestaurante(auxENTERO, nuevoNombre, nuevaDesc) == 1)
        {
            printf("¡Restaurante modificado con exito!\n");
        }
        else
        {
            printf("Error al intentar modificar.\n");
        }
    }
}

void pantallaBajaRestaurante()
{
    int auxENTERO = 0;
    printf("\n--- BAJA DE RESTAURANTE ---\n");
    printf("Ingrese el ID del restaurante a dar de baja: ");
    scanf("%d", &auxENTERO);
    getchar();

    if(buscarRestaurantePorID(auxENTERO) == 0)
    {
        puts("Error: El ID no existe o ya esta de baja.");
    }
    else
    {
        if(bajaRestaurante(auxENTERO) == 1)
        {
            printf("¡Restaurante dado de baja exitosamente!\n");
        }
        else
        {
            printf("Error al intentar dar de baja.\n");
        }
    }
}

void gestionRestaurantes()
{
    int opcABM;
    do
    {
        printf("\n--- ABM RESTAURANTES ---\n");
        printf("1. Alta de Restaurante\n");
        printf("2. Modificar Restaurante\n");
        printf("3. Baja de Restaurante\n");
        printf("0. Volver al menu principal\n");
        printf("Opcion ABM: ");
        scanf("%d", &opcABM);
        getchar();

        switch(opcABM)
        {
        case 1:
            pantallaAltaRestaurante();
            break
