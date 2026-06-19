#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "estructuras.h"
#include "crud.h"
#include "filemanager.h"

// ==========================================================
// --- AUXILIARES INTERNOS DE PERSISTENCIA Y MATEMÁTICA ---
// ==========================================================

int generarIdDetalleAutoincremental() {
    FILE *f = fopen("detalles_pedido.dat", "rb");
    if(f == NULL) return 1;
    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    fclose(f);
    return (int)(tam / sizeof(ProductosPedido)) + 1;
}

float obtenerPrecioProducto(int id_restaurante, int id_producto) {
    FILE *f = fopen("productos.dat", "rb");
    if(f == NULL) return 0.0;
    Producto aux;
    float precio = 0.0;
    while(leerRegistro(f, &aux, sizeof(Producto))) {
        if(aux.id_usuario_restaurante == id_restaurante && aux.id_producto == id_producto && aux.activo == 1) {
            precio = aux.precio;
            break;
        }
    }
    fclose(f);
    return precio;
}

// ==========================================
// --- MÓDULO 1: PLATOS (PRODUCTOS) --------
// ==========================================

int checkEstadoProductoGlobal(int id_restaurante, int id_producto) {
    FILE *f = fopen("productos.dat", "rb");
    if(f == NULL) return 0;
    Producto aux;
    while(leerRegistro(f, &aux, sizeof(Producto))) {
        if(aux.id_usuario_restaurante == id_restaurante && aux.id_producto == id_producto) {
            fclose(f);
            return aux.activo == 1 ? 1 : 2;
        }
    }
    fclose(f);
    return 0;
}

int reactivarProductoSimple(int id_restaurante, int id_producto) {
    FILE *f = fopen("productos.dat", "rb+");
    if(f == NULL) return 0;
    Producto aux;
    int reactivado = 0;
    while(leerRegistro(f, &aux, sizeof(Producto))) {
        if(aux.id_usuario_restaurante == id_restaurante && aux.id_producto == id_producto && aux.activo == 0) {
            aux.activo = 1;
            aux.estado = 1;
            fseek(f, -(long)sizeof(Producto), SEEK_CUR);
            reactivado = escribirRegistro(f, &aux, sizeof(Producto));
            break;
        }
    }
    fclose(f);
    return reactivado;
}

int reactivarYModificarProducto(Producto p) {
    FILE *f = fopen("productos.dat", "rb+");
    if(f == NULL) return 0;
    Producto aux;
    int exito = 0;
    while(leerRegistro(f, &aux, sizeof(Producto))) {
        if(aux.id_usuario_restaurante == p.id_usuario_restaurante && aux.id_producto == p.id_producto) {
            fseek(f, -(long)sizeof(Producto), SEEK_CUR);
            exito = escribirRegistro(f, &p, sizeof(Producto));
            break;
        }
    }
    fclose(f);
    return exito;
}

void menuReactivarProducto(int id_rest) {
    int auxENTERO;
    printf("\n--- REACTIVAR PLATO ---\n");
    printf("Ingrese el ID del plato a reactivar: ");
    scanf("%d", &auxENTERO);
    getchar();

    int estado = checkEstadoProductoGlobal(id_rest, auxENTERO);
    if(estado == 0) printf("Error: ID inexistente.\n");
    else if(estado == 1) printf("Aviso: El plato ya esta activo.\n");
    else {
        if(reactivarProductoSimple(id_rest, auxENTERO) == 1) printf("¡Plato revivido en el menu!\n");
        else printf("Error al reactivar.\n");
    }
}

void menuAltaProducto(int id_rest) {
    Producto p;
    memset(&p, 0, sizeof(Producto));
    int auxENTERO, invalido, estado_id, es_reactivacion = 0;
    char auxCARACTER[150];

    printf("\n--- ALTA DE PLATO ---\n");
    do {
        printf("ID del nuevo Plato: ");
        scanf("%d", &auxENTERO);
        getchar();
        estado_id = checkEstadoProductoGlobal(id_rest, auxENTERO);

        if(estado_id == 1) {
            puts("Ese ID ya existe y esta ACTIVO. Ingrese otro.");
            invalido = 1;
        } else if(estado_id == 2) {
            printf("-> ID dado de BAJA. Se reactivara con nuevos datos.\n");
            es_reactivacion = 1;
            invalido = 0;
        } else {
            es_reactivacion = 0;
            invalido = 0;
        }
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
        scanf("%f", &p.precio);
        getchar();
        invalido = (p.precio <= 0);
        if(invalido) puts("El precio debe ser mayor a 0");
    } while(invalido);

    p.estado = 1;
    p.activo = 1;

    if(es_reactivacion) {
        if(reactivarYModificarProducto(p) == 1) printf("¡Plato reactivado!\n");
    } else {
        if(guardarProducto(p) == 1) printf("¡Plato guardado!\n");
    }
}

void menuModificarProducto(int id_rest) {
    int auxENTERO, invalido;
    char nuevoNombre[50];
    float nuevoPrecio;
    printf("\n--- MODIFICAR PLATO ---\n");
    printf("Ingrese ID del plato: ");
    scanf("%d", &auxENTERO);
    getchar();

    if(buscarProductoPorID(id_rest, auxENTERO) == 0) {
        puts("Error: El plato no existe.");
    } else {
        do {
            printf("Nuevo Nombre: ");
            fgets(nuevoNombre, sizeof(nuevoNombre), stdin);
            nuevoNombre[strcspn(nuevoNombre, "\n")] = '\0';
            invalido = validarNombre(nuevoNombre) == 0;
            if(invalido) puts("Nombre invalido");
        } while(invalido);

        do {
            printf("Nuevo Precio: ");
            scanf("%f", &nuevoPrecio);
            getchar();
            invalido = (nuevoPrecio <= 0);
            if(invalido) puts("El precio debe ser mayor a 0");
        } while(invalido);

        if(modificarProducto(id_rest, auxENTERO, nuevoNombre, nuevoPrecio) == 1) printf("¡Modificado!\n");
    }
}

void menuBajaProducto(int id_rest) {
    int id_p;
    printf("\n--- BAJA DE PLATO ---\n");
    printf("ID del plato a dar de baja: ");
    scanf("%d", &id_p);
    getchar();

    if(bajaProducto(id_rest, id_p) == 1) printf("¡Plato dado de baja!\n");
    else printf("Error: Plato no encontrado.\n");
}

void gestionPlatosRestoEspecifico(int id_rest) {
    int opc;
    do {
        printf("\n--- MENU DE PLATOS - RESTAURANTE #%d ---\n", id_rest);
        printf("1. Alta de Plato\n");
        printf("2. Modificar Plato\n");
        printf("3. Baja de Plato\n");
        printf("4. Reactivar Plato\n");
        printf("0. Volver\n");
        printf("Opcion: ");
        scanf("%d", &opc);
        getchar();

        switch(opc) {
            case 1: menuAltaProducto(id_rest); break;
            case 2: menuModificarProducto(id_rest); break;
            case 3: menuBajaProducto(id_rest); break;
            case 4: menuReactivarProducto(id_rest); break;
        }
    } while(opc != 0);
}

int buscarProductoPorID(int id_restaurante, int id_producto) {
    FILE *f = fopen("productos.dat", "rb");
    if(f == NULL) return 0;
    Producto aux;
    while(leerRegistro(f, &aux, sizeof(Producto))) {
        if(aux.id_usuario_restaurante == id_restaurante && aux.id_producto == id_producto && aux.activo == 1) {
            fclose(f); return 1;
        }
    }
    fclose(f); return 0;
}

int guardarProducto(Producto p) {
    FILE *f = fopen("productos.dat", "ab");
    if(f == NULL) return 0;
    int res = escribirRegistro(f, &p, sizeof(Producto));
    fclose(f); return res;
}

int modificarProducto(int id_restaurante, int id_producto, char nuevoNombre[], float nuevoPrecio) {
    FILE *f = fopen("productos.dat", "rb+");
    if(f == NULL) return 0;
    Producto aux;
    int mod = 0;
    while(leerRegistro(f, &aux, sizeof(Producto))) {
        if(aux.id_usuario_restaurante == id_restaurante && aux.id_producto == id_producto && aux.activo == 1) {
            strcpy(aux.nombre, nuevoNombre);
            aux.precio = nuevoPrecio;
            fseek(f, -(long)sizeof(Producto), SEEK_CUR);
            mod = escribirRegistro(f, &aux, sizeof(Producto));
            break;
        }
    }
    fclose(f); return mod;
}

int bajaProducto(int id_restaurante, int id_producto) {
    FILE *f = fopen("productos.dat", "rb+");
    if(f == NULL) return 0;
    Producto aux;
    int borrado = 0;
    while(leerRegistro(f, &aux, sizeof(Producto))) {
        if(aux.id_usuario_restaurante == id_restaurante && aux.id_producto == id_producto && aux.activo == 1) {
            aux.activo = 0;
            fseek(f, -(long)sizeof(Producto), SEEK_CUR);
            borrado = escribirRegistro(f, &aux, sizeof(Producto));
            break;
        }
    }
    fclose(f); return borrado;
}

// ==========================================
// --- MÓDULO 2: RESTAURANTES --------------
// ==========================================

int checkEstadoRestauranteGlobal(int id) {
    FILE *f = fopen("restaurantes.dat", "rb");
    if(f == NULL) return 0;
    Restaurante aux;
    while(leerRegistro(f, &aux, sizeof(Restaurante))) {
        if(aux.id_restaurante == id) {
            fclose(f); return aux.activo == 1 ? 1 : 2;
        }
    }
    fclose(f); return 0;
}

int reactivarRestauranteSimple(int id) {
    FILE *f = fopen("restaurantes.dat", "rb+");
    if(f == NULL) return 0;
    Restaurante aux;
    int rec = 0;
    while(leerRegistro(f, &aux, sizeof(Restaurante))) {
        if(aux.id_restaurante == id && aux.activo == 0) {
            aux.activo = 1;
            fseek(f, -(long)sizeof(Restaurante), SEEK_CUR);
            rec = escribirRegistro(f, &aux, sizeof(Restaurante));
            break;
        }
    }
    fclose(f); return rec;
}

void pantallaReactivarRestaurante() {
    int idb;
    printf("\n--- REACTIVAR RESTAURANTE ---\n");
    printf("ID a reactivar: ");
    scanf("%d", &idb);
    getchar();
    int est = checkEstadoRestauranteGlobal(idb);
    if(est == 2 && reactivarRestauranteSimple(idb)) printf("¡Reactivado!\n");
    else printf("No se pudo reactivar o ya esta activo.\n");
}

void pantallaAltaRestaurante() {
    Restaurante r;
    memset(&r, 0, sizeof(Restaurante));
    int auxENTERO, invalido;
    char auxCARACTER[150];

    printf("\n--- ALTA DE RESTAURANTE ---\n");
    do {
        printf("ID Restaurante: ");
        scanf("%d", &auxENTERO);
        getchar();
        invalido = checkEstadoRestauranteGlobal(auxENTERO) == 1;
        if(invalido) puts("Ese ID ya existe, ingrese otro!");
    } while(invalido);
    r.id_restaurante = auxENTERO;

    do {
        printf("Nombre del Restaurante: ");
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
        auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = validarNombre(auxCARACTER) == 0;
        if(invalido) puts("Nombre invalido, intente de nuevo");
    } while(invalido);
    strcpy(r.nombre, auxCARACTER);

    do {
        printf("Direccion: "); // CAMBIADO: Solo "Direccion:"
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
        auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = strlen(auxCARACTER) == 0;
        if(invalido) puts("Debe llenar la direccion");
    } while(invalido);
    strcpy(r.direccion, auxCARACTER);

    do {
        printf("Descripcion / Rubro: ");
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
        auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = strlen(auxCARACTER) == 0;
        if(invalido) puts("Debe llenar el campo");
    } while(invalido);
    strcpy(r.descripcion, auxCARACTER);

    do {
        printf("Contrase%ca corporativa: ", 164); // Carga de clave del local
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
        auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = strlen(auxCARACTER) == 0;
        if(invalido) puts("Debe ingresar una contrasenia");
    } while(invalido);
    strcpy(r.contrasenia, auxCARACTER);

    r.calificacion = 0.0;
    r.activo = 1;

    guardarRestaurante(r);
    printf("¡Guardado!\n");
}

void pantallaModificacionRestaurante() {
    int idb, invalido;
    char nom[50], dir[150];
    printf("\n--- MODIFICAR RESTAURANTE ---\n");
    printf("ID: ");
    scanf("%d", &idb);
    getchar();

    if(buscarRestaurantePorID(idb)) {
        do {
            printf("Nuevo Nombre: ");
            fgets(nom, 50, stdin); nom[strcspn(nom, "\n")] = '\0';
            invalido = validarNombre(nom) == 0;
            if(invalido) puts("Nombre invalido");
        } while(invalido);

        do {
            printf("Nueva Direccion: ");
            fgets(dir, 150, stdin); dir[strcspn(dir, "\n")] = '\0';
            invalido = strlen(dir) == 0;
            if(invalido) puts("Debe llenar el campo");
        } while(invalido);

        modificarRestaurante(idb, nom, dir);
        printf("¡Modificado!\n");
    } else printf("No encontrado.\n");
}

void pantallaBajaRestaurante() {
    int idb;
    printf("\n--- BAJA DE RESTAURANTE ---\n");
    printf("ID: ");
    scanf("%d", &idb);
    getchar();
    if(bajaRestaurante(idb)) {
        printf("¡Dado de baja local y platos en cascada!\n");
    } else printf("Error.\n");
}

void gestionRestaurantes() {
    int opc;
    do {
        printf("\n--- PANEL ABM RESTAURANTES ---\n");
        printf("1. Alta\n2. Modificacion\n3. Baja\n4. Reactivar\n0. Volver\nOpcion: ");
        scanf("%d", &opc); getchar();
        if(opc==1) pantallaAltaRestaurante();
        if(opc==2) pantallaModificacionRestaurante();
        if(opc==3) pantallaBajaRestaurante();
        if(opc==4) pantallaReactivarRestaurante();
    } while(opc != 0);
}

int buscarRestaurantePorID(int id) {
    FILE *f = fopen("restaurantes.dat", "rb");
    if(f == NULL) return 0;
    Restaurante aux;
    while(leerRegistro(f, &aux, sizeof(Restaurante))) {
        if(aux.id_restaurante == id && aux.activo == 1) {
            fclose(f); return 1;
        }
    }
    fclose(f); return 0;
}

// Validación de login de locales por clave
int buscarRestoPorIdYContrasenia(int id, char pass[]) {
    FILE *f = fopen("restaurantes.dat", "rb");
    if(f == NULL) return 0;
    Restaurante aux;
    while(leerRegistro(f, &aux, sizeof(Restaurante))) {
        if(aux.id_restaurante == id && strcmp(aux.contrasenia, pass) == 0 && aux.activo == 1) {
            fclose(f); return 1;
        }
    }
    fclose(f); return 0;
}

int guardarRestaurante(Restaurante r) {
    FILE *f = fopen("restaurantes.dat", "ab");
    if(f == NULL) return 0;
    int res = escribirRegistro(f, &r, sizeof(Restaurante));
    fclose(f); return res;
}

int modificarRestaurante(int id, char nuevoNombre[], char nuevaDescripcion[]) {
    FILE *f = fopen("restaurantes.dat", "rb+");
    if(f == NULL) return 0;
    Restaurante aux;
    int mod = 0;
    while(leerRegistro(f, &aux, sizeof(Restaurante))) {
        if(aux.id_restaurante == id && aux.activo == 1) {
            strcpy(aux.nombre, nuevoNombre);
            strcpy(aux.direccion, nuevaDescripcion);
            fseek(f, -(long)sizeof(Restaurante), SEEK_CUR);
            mod = escribirRegistro(f, &aux, sizeof(Restaurante));
            break;
        }
    }
    fclose(f); return mod;
}

int bajaRestaurante(int id) {
    FILE *f = fopen("restaurantes.dat", "rb+");
    if(f == NULL) return 0;
    Restaurante aux;
    int borrado = 0;
    while(leerRegistro(f, &aux, sizeof(Restaurante))) {
        if(aux.id_restaurante == id && aux.activo == 1) {
            aux.activo = 0;
            fseek(f, -(long)sizeof(Restaurante), SEEK_CUR);
            borrado = escribirRegistro(f, &aux, sizeof(Restaurante));
            break;
        }
    }
    fclose(f);

    if(borrado) {
        FILE *fp = fopen("productos.dat", "rb+");
        if(fp != NULL) {
            Producto p;
            while(leerRegistro(fp, &p, sizeof(Producto))) {
                if(p.id_usuario_restaurante == id) {
                    p.activo = 0;
                    fseek(fp, -(long)sizeof(Producto), SEEK_CUR);
                    escribirRegistro(fp, &p, sizeof(Producto));
                }
            }
            fclose(fp);
        }
    }
    return borrado;
}

// ==========================================
// --- MÓDULO 3: CLIENTES ------------------
// ==========================================

int checkEstadoClienteGlobal(int id) {
    FILE *f = fopen("clientes.dat", "rb");
    if(f == NULL) return 0;
    Cliente aux;
    while(leerRegistro(f, &aux, sizeof(Cliente))) {
        if(aux.id_cliente == id) {
            fclose(f); return aux.activo == 1 ? 1 : 2;
        }
    }
    fclose(f); return 0;
}

int reactivarClienteSimple(int id) {
    FILE *f = fopen("clientes.dat", "rb+");
    if(f == NULL) return 0;
    Cliente aux;
    int rec = 0;
    while(leerRegistro(f, &aux, sizeof(Cliente))) {
        if(aux.id_cliente == id && aux.activo == 0) {
            aux.activo = 1;
            fseek(f, -(long)sizeof(Cliente), SEEK_CUR);
            rec = escribirRegistro(f, &aux, sizeof(Cliente));
            break;
        }
    }
    fclose(f); return rec;
}

void pantallaReactivarCliente() {
    int idb;
    printf("\n--- REACTIVAR CLIENTE ---\n");
    printf("ID: "); scanf("%d", &idb); getchar();
    if(checkEstadoClienteGlobal(idb) == 2 && reactivarClienteSimple(idb)) printf("¡Reactivado!\n");
    else printf("No operativo o ya se encuentra activo.\n");
}

void pantallaAltaCliente() {
    Cliente c; memset(&c, 0, sizeof(Cliente));
    int idb, invalido;
    char auxCARACTER[150];

    printf("\n--- ALTA DE CLIENTE ---\n");
    do {
        printf("ID Unico: "); scanf("%d", &idb); getchar();
        invalido = checkEstadoClienteGlobal(idb) == 1;
        if(invalido) puts("Ese ID ya existe, ingrese otro!");
    } while(invalido);
    c.id_cliente = idb;

    do {
        printf("Nombre: "); fgets(auxCARACTER, sizeof(auxCARACTER), stdin); auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = validarNombre(auxCARACTER) == 0;
        if(invalido) puts("Nombre invalido, intente de nuevo");
    } while(invalido);
    strcpy(c.nombre, auxCARACTER);

    do {
        printf("Email: "); fgets(auxCARACTER, sizeof(auxCARACTER), stdin); auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = validarEmail(auxCARACTER) == 0;
        if(invalido) puts("El email es invalido, intente de nuevo");
    } while(invalido);
    strcpy(c.email, auxCARACTER);

    do {
        printf("Telefono (Solo numeros): "); fgets(auxCARACTER, sizeof(auxCARACTER), stdin); auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = validarTelefono(auxCARACTER) == 0;
        if(invalido) puts("Telefono invalido. Ingrese solo numeros sin espacios ni letras.");
    } while(invalido);
    strcpy(c.telefono, auxCARACTER);

    do {
        printf("Contrase%ca de ingreso obligatoria: ", 164); fgets(auxCARACTER, sizeof(auxCARACTER), stdin); auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = strlen(auxCARACTER) == 0;
        if(invalido) puts("Debe llenar la contrasenia");
    } while(invalido);
    strcpy(c.contrasenia, auxCARACTER);

    do {
        printf("Direccion: "); // CAMBIADO: Solo "Direccion:"
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin); auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = strlen(auxCARACTER) == 0;
        if(invalido) puts("Debe llenar la direccion");
    } while(invalido);
    strcpy(c.direccion, auxCARACTER);

    c.activo = 1;
    guardarCliente(c);
    printf("¡Registrado con exito!\n");
}

void pantallaModificarCliente() {
    int idb, invalido;
    char dir[150], pass[50], tel[20];
    printf("\n--- MODIFICAR CLIENTE ---\n");
    printf("ID: "); scanf("%d", &idb); getchar();

    if(buscarClientePorID(idb)) {
        do {
            printf("Nueva Direccion: "); fgets(dir, 150, stdin); dir[strcspn(dir, "\n")] = '\0';
            invalido = strlen(dir) == 0;
            if(invalido) puts("Debe llenar el campo");
        } while(invalido);

        do {
            printf("Nueva Contrase%ca: ", 164); fgets(pass, 50, stdin); pass[strcspn(pass, "\n")] = '\0';
            invalido = strlen(pass) == 0;
            if(invalido) puts("Debe llenar el campo");
        } while(invalido);

        do {
            printf("Nuevo Telefono: "); fgets(tel, 20, stdin); tel[strcspn(tel, "\n")] = '\0';
            invalido = validarTelefono(tel) == 0;
            if(invalido) puts("Telefono invalido, intente de nuevo");
        } while(invalido);

        modificarClienteCompleto(idb, dir, pass, tel);
        printf("¡Actualizado!\n");
    } else printf("No encontrado o dado de baja.\n");
}

void pantallaBajaCliente() {
    int idb;
    printf("\n--- BAJA DE CLIENTE ---\n");
    printf("ID: "); scanf("%d", &idb); getchar();
    if(bajaCliente(idb)) printf("¡Baja procesada con exito!\n");
    else printf("Error.\n");
}

void gestionClientes() {
    int opc;
    do {
        printf("\n--- PANEL ABM CLIENTES ---\n");
        printf("1. Alta\n2. Modificacion\n3. Baja\n4. Reactivar\n0. Volver\nOpcion: ");
        scanf("%d", &opc); getchar();
        if(opc==1) pantallaAltaCliente();
        if(opc==2) pantallaModificarCliente();
        if(opc==3) pantallaBajaCliente();
        if(opc==4) pantallaReactivarCliente();
    } while(opc != 0);
}

int buscarClientePorID(int id) {
    FILE *f = fopen("clientes.dat", "rb");
    if(f == NULL) return 0;
    Cliente aux;
    while(leerRegistro(f, &aux, sizeof(Cliente))) {
        if(aux.id_cliente == id && aux.activo == 1) {
            fclose(f); return 1;
        }
    }
    fclose(f); return 0;
}

int buscarClientePorIdYContrasenia(int id, char pass[]) {
    FILE *f = fopen("clientes.dat", "rb");
    if(f == NULL) return 0;
    Cliente aux;
    while(leerRegistro(f, &aux, sizeof(Cliente))) {
        if(aux.id_cliente == id && strcmp(aux.contrasenia, pass) == 0 && aux.activo == 1) {
            fclose(f); return 1;
        }
    }
    fclose(f); return 0;
}

int guardarCliente(Cliente c) {
    FILE *f = fopen("clientes.dat", "ab");
    if(f == NULL) return 0;
    int res = escribirRegistro(f, &c, sizeof(Cliente));
    fclose(f); return res;
}

int modificarClienteCompleto(int id, char nuevaDireccion[], char nuevaContrasenia[], char nuevoTelefono[]) {
    FILE *f = fopen("clientes.dat", "rb+");
    if(f == NULL) return 0;
    Cliente aux; int mod = 0;
    while(leerRegistro(f, &aux, sizeof(Cliente))) {
        if(aux.id_cliente == id && aux.activo == 1) {
            strcpy(aux.direccion, nuevaDireccion);
            strcpy(aux.contrasenia, nuevaContrasenia);
            strcpy(aux.telefono, nuevoTelefono);
            fseek(f, -(long)sizeof(Cliente), SEEK_CUR);
            mod = escribirRegistro(f, &aux, sizeof(Cliente));
            break;
        }
    }
    fclose(f); return mod;
}

int modificarCliente(int id, char nuevaDireccion[], char nuevaContrasenia[]) {
    return modificarClienteCompleto(id, nuevaDireccion, nuevaContrasenia, "");
}

int bajaCliente(int id) {
    FILE *f = fopen("clientes.dat", "rb+");
    if(f == NULL) return 0;
    Cliente aux; int borrado = 0;
    while(leerRegistro(f, &aux, sizeof(Cliente))) {
        if(aux.id_cliente == id && aux.activo == 1) {
            aux.activo = 0;
            fseek(f, -(long)sizeof(Cliente), SEEK_CUR);
            borrado = escribirRegistro(f, &aux, sizeof(Cliente));
            break;
        }
    }
    fclose(f); return borrado;
}

// ==========================================
// --- MÓDULO 4: OPERACIONES DE PEDIDOS -----
// ==========================================

int altaPedido(PedidoCliente nuevoPedido, ProductosPedido detalles[], int cantidadProductos) {
    FILE *f = fopen("pedidos.dat", "ab");
    if(f == NULL) return 0;
    escribirRegistro(f, &nuevoPedido, sizeof(PedidoCliente));
    fclose(f);

    FILE *fd = fopen("detalles_pedido.dat", "ab");
    if(fd != NULL) {
        int idD = generarIdDetalleAutoincremental();
        for(int i = 0; i < cantidadProductos; i++) {
            detalles[i].id_detalle = idD;
            detalles[i].id_pedido = nuevoPedido.id_pedido;
            fwrite(&detalles[i], sizeof(ProductosPedido), 1, fd);
            idD++;
        }
        fclose(fd);
    }
    return 1;
}

void pantallaAltaPedidoCliente(int id_logueado) {
    PedidoCliente p; memset(&p, 0, sizeof(PedidoCliente));
    ProductosPedido detalles[50];
    int cantP = 0, auxID, inv; char sigue;

    p.id_cliente = id_logueado;
    printf("\n--- NUEVO PEDIDO ---\n");

    // 1. Validar Restaurante y Mostrar sus datos en vivo
    do {
        printf("ID del Restaurante donde comprara: "); scanf("%d", &auxID); getchar();

        // Buscar y mostrar nombre comercial
        FILE *fr = fopen("restaurantes.dat", "rb");
        inv = 1;
        if(fr != NULL) {
            Restaurante r;
            while(leerRegistro(fr, &r, sizeof(Restaurante))) {
                if(r.id_restaurante == auxID && r.activo == 1) {
                    printf("\n>>> COMPRANDO EN: %s <<<\n", r.nombre);
                    inv = 0;
                    break;
                }
            }
            fclose(fr);
        }
        if(inv) puts("Restaurante invalido o inactivo. Intente de nuevo.");
    } while(inv);
    p.id_usuario_restaurante = auxID;

    // 2. Listar automáticamente los platos cargados de este restaurante
    printf("\n--- MENU DISPONIBLE DEL LOCAL ---\n");
    FILE *fp = fopen("productos.dat", "rb");
    int tiene_platos = 0;
    if(fp != NULL) {
        Producto prod;
        while(leerRegistro(fp, &prod, sizeof(Producto))) {
            if(prod.id_usuario_restaurante == p.id_usuario_restaurante && prod.activo == 1) {
                printf("  ID: %d | %s | Desc: %s | Precio: $%.2f\n",
                       prod.id_producto, prod.nombre, prod.descripcion, prod.precio);
                tiene_platos = 1;
            }
        }
        fclose(fp);
    }
    if(!tiene_platos) {
        printf("Aviso: Este restaurante no tiene platos activos en este momento.\n");
        return;
    }
    printf("---------------------------------\n");

    printf("ID unico para el ticket del pedido: "); scanf("%d", &p.id_pedido); getchar();
    strcpy(p.fecha, "2026-06-19");

    printf("Direccion de entrega: ");
    fgets(p.ubicacion_entrega, 100, stdin); p.ubicacion_entrega[strcspn(p.ubicacion_entrega, "\n")] = '\0';
    strcpy(p.codigo_envio, "ENV-AUTO");
    p.estado = 1; p.activo = 1;

    printf("\n--- ARME SU CARRITO ---\n");
    p.total = 0.0;
    do {
        do {
            printf("Ingrese ID del Plato a llevar: "); scanf("%d", &auxID); getchar();
            inv = buscarProductoPorID(p.id_usuario_restaurante, auxID) == 0;
            if(inv) puts("Ese ID de plato no existe en este restaurante.");
        } while(inv);

        detalles[cantP].id_producto = auxID;
        printf("Cantidad: "); scanf("%d", &detalles[cantP].cantidad); getchar();

        float unitario = obtenerPrecioProducto(p.id_usuario_restaurante, auxID);
        detalles[cantP].subtotal = unitario * detalles[cantP].cantidad;
        p.total += detalles[cantP].subtotal;
        cantP++;

        printf("Monto acumulado: $%.2f. ¿Sumar otro plato? (s/n): ", p.total);
        scanf("%c", &sigue); getchar();
    } while((sigue=='s'||sigue=='S') && cantP < 50);

    if(altaPedido(p, detalles, cantP)) printf("¡Pedido enviado al restaurante con exito!\n");
}

void pantallaListarPedidosUnicoCliente(int id_logueado) {
    FILE *f = fopen("pedidos.dat", "rb");
    if(f == NULL) { printf("Sin movimientos registrados.\n"); return; }
    PedidoCliente p; int enc = 0;

    printf("\n=== HISTORIAL DE SUS PEDIDOS (CLIENTE #%d) ===\n", id_logueado);
    while(fread(&p, sizeof(PedidoCliente), 1, f) == 1) {
        if(p.id_cliente == id_logueado && p.activo == 1) {
            enc++;
            printf("ID Pedido: #%d | Local ID: %d | Total: $%.2f | Estado: %s\n",
                   p.id_pedido, p.id_usuario_restaurante, p.total,
                   p.estado==1?"Pendiente":p.estado==2?"En Viaje":"Entregado");
        }
    }
    fclose(f);
    if(!enc) { printf("Sin movimientos.\n"); return; }

    // Opción interactiva de feedback requerida por Lucía
    printf("\n¿Desea calificar algun pedido de su lista? (s/n): ");
    char calif_opc;
    scanf("%c", &calif_opc); getchar();

    if(calif_opc == 's' || calif_opc == 'S') {
        int id_ped_buscar, estrellas, encontrado_y_valido = 0, id_resto_calificar = 0;
        printf("Ingrese el ID del pedido a calificar: ");
        scanf("%d", &id_ped_buscar); getchar();

        // Verificar que el pedido le pertenezca y esté efectivamente Entregado (3)
        f = fopen("pedidos.dat", "rb");
        if(f != NULL) {
            while(fread(&p, sizeof(PedidoCliente), 1, f) == 1) {
                if(p.id_pedido == id_ped_buscar && p.id_cliente == id_logueado && p.activo == 1) {
                    if(p.estado == 3) {
                        encontrado_y_valido = 1;
                        id_resto_calificar = p.id_usuario_restaurante;
                    } else {
                        printf("Aviso: No puede calificar este pedido porque todavia esta '%s'.\n",
                               p.estado == 1 ? "Pendiente" : "En Viaje");
                    }
                    break;
                }
            }
            fclose(f);
        }

        if(encontrado_y_valido) {
            do {
                printf("Ingrese su Calificacion para el restaurante (1 al 5 estrellas): ");
                scanf("%d", &estrellas); getchar();
            } while(estrellas < 1 || estrellas > 5);

            // Algoritmo de actualización in-situ de promedios comerciales corporativos
            FILE *fr = fopen("restaurantes.dat", "rb+");
            if(fr != NULL) {
                Restaurante r;
                int modificado = 0;
                while(leerRegistro(fr, &r, sizeof(Restaurante))) {
                    if(r.id_restaurante == id_resto_calificar && r.activo == 1) {
                        r.calificacion = (r.calificacion == 0.0) ? (float)estrellas : (r.calificacion + estrellas) / 2.0;
                        fseek(fr, -(long)sizeof(Restaurante), SEEK_CUR);
                        escribirRegistro(fr, &r, sizeof(Restaurante));
                        modificado = 1;
                        break;
                    }
                }
                fclose(fr);
                if(modificado) printf("¡Muchas gracias por su valoracion! El promedio del local fue actualizado.\n");
            }
        } else if(id_resto_calificar == 0) {
            printf("Error: ID de ticket inexistente o no pertenece a su usuario.\n");
        }
    }
}

void gestionPedidosRestoEspecifico(int id_rest) {
    FILE *f = fopen("pedidos.dat", "rb");
    if(f == NULL) { printf("Sin pedidos.\n"); return; }
    PedidoCliente p; int opc, idb, enc = 0;

    printf("\n=== PEDIDOS DE SU RESTAURANTE (REST #%d) ===\n", id_rest);
    while(fread(&p, sizeof(PedidoCliente), 1, f) == 1) {
        if(p.id_usuario_restaurante == id_rest && p.activo == 1) {
            enc++;
            printf("ID Pedido: %d | Cliente: %d | Monto: $%.2f | Estado actual: %s\n",
                   p.id_pedido, p.id_cliente, p.total,
                   p.estado==1?"Pendiente":p.estado==2?"En Viaje":"Entregado");
        }
    }
    fclose(f);
    if(!enc) { printf("No tiene ningun pedido asignado.\n"); return; }

    printf("\n1. Modificar Estado de un Pedido\n0. Volver\nOpcion: ");
    scanf("%d", &opc); getchar();
    if(opc == 1) {
        printf("ID del pedido a cambiar de estado: "); scanf("%d", &idb); getchar();
        printf("Nuevo Estado (2=En Viaje, 3=Entregado): "); scanf("%d", &opc); getchar();

        if(modificarEstadoPedido(idb, opc)) {
            printf("¡Estado cambiado con exito!\n");
            // Se eliminó la solicitud de calificación desde acá.
        } else {
            printf("Error al actualizar el estado del pedido.\n");
        }
    }
}

int modificarEstadoPedido(int id_pedido, int nuevoEstado) {
    FILE *f = fopen("pedidos.dat", "rb+");
    if(f == NULL) return 0;
    PedidoCliente p; int mod = 0;
    while(leerRegistro(f, &p, sizeof(PedidoCliente))) {
        if(p.id_pedido == id_pedido && p.activo == 1) {
            p.estado = nuevoEstado;
            fseek(f, -(long)sizeof(PedidoCliente), SEEK_CUR);
            mod = escribirRegistro(f, &p, sizeof(PedidoCliente));
            break;
        }
    }
    fclose(f); return mod;
}

int bajaPedido(int id_pedido) {
    FILE *f = fopen("pedidos.dat", "rb+");
    if(f == NULL) return 0;
    PedidoCliente p; int mod = 0;
    while(leerRegistro(f, &p, sizeof(PedidoCliente))) {
        if(p.id_pedido == id_pedido && p.activo == 1) {
            p.activo = 0;
            fseek(f, -(long)sizeof(PedidoCliente), SEEK_CUR);
            mod = escribirRegistro(f, &p, sizeof(PedidoCliente));
            break;
        }
    }
    fclose(f); return mod;
}

// ==========================================
// --- MÓDULO 5: VALIDACIONES BÁSICAS ------
// ==========================================

int validarTelefono(char telefono[]) {
    int len = strlen(telefono);
    if(len < 6 || len > 15) return 0;
    for(int i = 0; i < len; i++) {
        if(!isdigit(telefono[i])) return 0;
    }
    return 1;
}

int validarNombre(char nombre[]) {
    int len = strlen(nombre);
    if(len == 0) return 0;
    for(int i = 0; i < len; i++) {
        if(!isalpha(nombre[i]) && nombre[i] != ' ') return 0;
    }
    return 1;
}

int validarEmail(char email[]) {
    if(strlen(email) == 0) return 0;
    char *arroba = strchr(email, '@');
    char *punto = strrchr(email, '.');
    return (arroba != NULL && punto != NULL && arroba + 1 < punto);
}

int validarFecha(char fecha[]) {
    if(strlen(fecha) != 10 || fecha[4] != '-' || fecha[7] != '-') return 0;
    int a, m, d;
    if(sscanf(fecha, "%d-%d-%d", &a, &m, &d) != 3) return 0;
    return (a >= 2000 && a <= 2100 && m >= 1 && m <= 12 && d >= 1 && d <= 31);
}

// ==========================================
// --- MÓDULO 6: LOGS DE DEBUG INTERNO -----
// ==========================================

void listarClientesDebug() {
    FILE *f = fopen("clientes.dat", "rb"); if(f == NULL) return;
    Cliente a;
    while(leerRegistro(f, &a, sizeof(Cliente))) {
        printf("ID: %d | %s | %s | Activo: %d\n", a.id_cliente, a.nombre, a.telefono, a.activo);
    }
    fclose(f);
}

void listarRestaurantesDebug() {
    FILE *f = fopen("restaurantes.dat", "rb"); if(f == NULL) return;
    Restaurante a;
    while(fread(&a, sizeof(Restaurante), 1, f) == 1) {
        printf("ID: %d | %s | Calif: %.1f | Activo: %d\n", a.id_restaurante, a.nombre, a.calificacion, a.activo);
    }
    fclose(f);
}

void listarProductosDebug() {
    FILE *f = fopen("productos.dat", "rb"); if(f == NULL) return;
    Producto a;
    while(fread(&a, sizeof(Producto), 1, f) == 1) {
        printf("ID Plt: %d | Resto: %d | %s | $%.2f | Activo: %d\n", a.id_producto, a.id_usuario_restaurante, a.nombre, a.precio, a.activo);
    }
    fclose(f);
}

void listarPedidosDebug() {
    FILE *f = fopen("pedidos.dat", "rb"); if(f == NULL) return;
    PedidoCliente a;
    while(fread(&a, sizeof(PedidoCliente), 1, f) == 1) {
        printf("Ticket: %d | Clie: %d | Resto: %d | $%.2f | Estado: %d\n", a.id_pedido, a.id_cliente, a.id_usuario_restaurante, a.total, a.estado);
    }
    fclose(f);
}
