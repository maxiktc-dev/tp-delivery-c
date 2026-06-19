#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "estructuras.h"
#include "crud.h"
#include "filemanager.h"

// ==========================================
// --- ABM Y LÓGICA DE PLATOS (PRODUCTOS) ---
// ==========================================

void menuAltaProducto(int id_rest)
{
    Producto p;
    int auxENTERO = 0;
    char auxCARACTER[150];
    float auxFLOAT = 0.0;
    int invalido = 0;

    printf("\n--- ALTA DE PLATO ---\n");
    do
    {
        printf("ID del nuevo Plato: ");
        scanf("%d", &auxENTERO);
        getchar();
        invalido = buscarProductoPorID(id_rest, auxENTERO) == 1;
        if(invalido) puts("Ese ID de plato ya existe en este restaurante!");
    }
    while(invalido);

    p.id_producto = auxENTERO;
    p.id_usuario_restaurante = id_rest;

    do
    {
        printf("Nombre del Plato: ");
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
        auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = validarNombre(auxCARACTER) == 0;
        if(invalido) puts("Nombre invalido, intente de nuevo");
    }
    while(invalido);
    strcpy(p.nombre, auxCARACTER);

    do
    {
        printf("Descripcion del Plato: ");
        fgets(p.descripcion, sizeof(p.descripcion), stdin);
        p.descripcion[strcspn(p.descripcion, "\n")] = '\0';
        invalido = strlen(p.descripcion) == 0;
        if(invalido) puts("Debe llenar la descripcion");
    }
    while(invalido);

    do
    {
        printf("Precio: ");
        scanf("%f", &auxFLOAT);
        getchar();
        invalido = (auxFLOAT <= 0);
        if(invalido) puts("El precio debe ser mayor a 0");
    }
    while(invalido);
    p.precio = auxFLOAT;

    p.estado = 1; // Disponible
    p.activo = 1; // Activo

    if(guardarProducto(p) == 1)
    {
        printf("¡Plato guardado exitosamente en el menu!\n");
    }
    else
    {
        printf("Error: No se pudo guardar el plato.\n");
    }
}

void menuModificarProducto(int id_rest)
{
    int auxENTERO = 0;
    char nuevoNombre[50];
    float auxFLOAT = 0.0;
    int invalido = 0;

    printf("\n--- MODIFICAR PLATO ---\n");
    printf("Ingrese el ID del plato a modificar: ");
    scanf("%d", &auxENTERO);
    getchar();

    if(buscarProductoPorID(id_rest, auxENTERO) == 0)
    {
        puts("Error: El plato no existe en este restaurante.");
    }
    else
    {
        do
        {
            printf("Nuevo Nombre del Plato: ");
            fgets(nuevoNombre, sizeof(nuevoNombre), stdin);
            nuevoNombre[strcspn(nuevoNombre, "\n")] = '\0';
            invalido = validarNombre(nuevoNombre) == 0;
            if(invalido) puts("Nombre invalido");
        }
        while(invalido);

        do
        {
            printf("Nuevo Precio: ");
            scanf("%f", &auxFLOAT);
            getchar();
            invalido = (auxFLOAT <= 0);
            if(invalido) puts("El precio debe ser mayor a 0");
        }
        while(invalido);

        if(modificarProducto(id_rest, auxENTERO, nuevoNombre, auxFLOAT) == 1)
        {
            printf("¡Plato modificado con exito!\n");
        }
        else
        {
            printf("Error al intentar modificar.\n");
        }
    }
}

void menuBajaProducto(int id_rest)
{
    int auxENTERO = 0;

    printf("\n--- BAJA DE PLATO ---\n");
    printf("Ingrese el ID del plato a dar de baja: ");
    scanf("%d", &auxENTERO);
    getchar();

    if(buscarProductoPorID(id_rest, auxENTERO) == 0)
    {
        puts("Error: El plato no existe en este restaurante.");
    }
    else
    {
        if(bajaProducto(id_rest, auxENTERO) == 1)
        {
            printf("¡Plato dado de baja del menu exitosamente!\n");
        }
        else
        {
            printf("Error al intentar dar de baja.\n");
        }
    }
}

void gestionPlatos()
{
    int id_rest, opcABM;

    printf("\n--- GESTION DE PLATOS (PRODUCTOS) ---\n");
    printf("Ingrese el ID del Restaurante due%co del menu: ", 164);
    scanf("%d", &id_rest);
    getchar();

    // Validamos primero que el restaurante exista y este activo
    if(buscarRestaurantePorID(id_rest) == 0)
    {
        puts("Error: El restaurante no existe o esta de baja.");
        return;
    }

    do
    {
        printf("\n--- MENU DEL RESTAURANTE #%d ---\n", id_rest);
        printf("1. Alta de Plato\n");
        printf("2. Modificar Plato\n");
        printf("3. Baja de Plato\n");
        printf("0. Volver al menu principal\n");
        printf("Opcion ABM: ");
        scanf("%d", &opcABM);
        getchar();

        switch(opcABM)
        {
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
    }
    while(opcABM != 0);
}

int buscarProductoPorID(int id_restaurante, int id_producto)
{
    FILE *f = fopen("productos.dat", "rb");
    if(f == NULL) return 0;

    Producto aux;
    while(leerRegistro(f, &aux, sizeof(Producto)))
    {
        if(aux.id_usuario_restaurante == id_restaurante && aux.id_producto == id_producto && aux.activo == 1)
        {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int guardarProducto(Producto p)
{
    FILE *f = abrirArchivo("productos.dat", "ab");
    if(f == NULL) return 0;

    int resultado = escribirRegistro(f, &p, sizeof(Producto));
    fclose(f);
    return resultado;
}

int modificarProducto(int id_restaurante, int id_producto, char nuevoNombre[], float nuevoPrecio)
{
    FILE *f = abrirArchivo("productos.dat", "rb+");
    if(f == NULL) return 0;

    Producto aux;
    int modificado = 0;

    while(leerRegistro(f, &aux, sizeof(Producto)))
    {
        if(aux.id_usuario_restaurante == id_restaurante && aux.id_producto == id_producto && aux.activo == 1)
        {
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

int bajaProducto(int id_restaurante, int id_producto)
{
    FILE *f = abrirArchivo("productos.dat", "rb+");
    if(f == NULL) return 0;

    Producto aux;
    int dadoDeBaja = 0;

    while(leerRegistro(f, &aux, sizeof(Producto)))
    {
        if(aux.id_usuario_restaurante == id_restaurante && aux.id_producto == id_producto && aux.activo == 1)
        {
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
            break;
        case 2:
            pantallaModificacionRestaurante(); // Corregido el nombre de la llamada
            break;
        case 3:
            pantallaBajaRestaurante();
            break;
        case 0:
            printf("Volviendo...\n");
            break;
        default:
            printf("Opcion invalida.\n");
        }
    }
    while(opcABM != 0);
}

int buscarRestaurantePorID(int id)
{
    FILE *f = abrirArchivo("restaurantes.dat", "rb");
    if(f == NULL) return 0;

    Restaurante aux;
    while(leerRegistro(f, &aux, sizeof(Restaurante)))
    {
        if(aux.id_restaurante == id && aux.activo == 1)
        {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int guardarRestaurante(Restaurante r)
{
    FILE *f = abrirArchivo("restaurantes.dat", "ab");
    if(f == NULL) return 0;

    int resultado = escribirRegistro(f, &r, sizeof(Restaurante));
    fclose(f);
    return resultado;
}

int modificarRestaurante(int id, char nuevoNombre[], char nuevaDescripcion[])
{
    FILE *f = abrirArchivo("restaurantes.dat", "rb+");
    if(f == NULL) return 0;

    Restaurante aux;
    int modificado = 0;

    while(leerRegistro(f, &aux, sizeof(Restaurante)))
    {
        if(aux.id_restaurante == id && aux.activo == 1)
        {
            strcpy(aux.nombre, nuevoNombre);
            strcpy(aux.descripcion, nuevaDescripcion);

            fseek(f, -sizeof(Restaurante), SEEK_CUR);
            modificado = escribirRegistro(f, &aux, sizeof(Restaurante));
            break;
        }
    }
    fclose(f);
    return modificado;
}

int bajaRestaurante(int id)
{
    FILE *f = abrirArchivo("restaurantes.dat", "rb+");
    if(f == NULL) return 0;

    Restaurante aux;
    int dadoDeBaja = 0;

    while(leerRegistro(f, &aux, sizeof(Restaurante)))
    {
        if(aux.id_restaurante == id && aux.activo == 1)
        {
            aux.activo = 0;

            fseek(f, -sizeof(Restaurante), SEEK_CUR);
            dadoDeBaja = escribirRegistro(f, &aux, sizeof(Restaurante));
            break;
        }
    }
    fclose(f);
    return dadoDeBaja;
}

// ==========================================
// --- 1. ABM Y LÓGICA DE CLIENTES ---
// ==========================================

void pantallaAltaCliente()
{
    Cliente c;
    int auxENTERO, invalido;
    char auxCARACTER[150];
    printf("\n--- ALTA DE CLIENTE ---\n");
    do
    {
        printf("ID: ");
        scanf("%d", &auxENTERO);
        getchar();
        invalido = buscarClientePorID(auxENTERO) == 1;
        if(invalido) puts("Ese ID ya existe, ingrese otro!");
    }
    while(invalido);
    c.id_cliente = auxENTERO;

    do
    {
        printf("Nombre: ");
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
        auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = validarNombre(auxCARACTER) == 0;
        if(invalido) puts("Nombre invalido, intente devuelta");
    }
    while(invalido);
    strcpy(c.nombre, auxCARACTER);

    do
    {
        printf("Email: ");
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
        auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = validarEmail(auxCARACTER) == 0;
        if(invalido) puts("El email es invalido, intente denuevo");
    }
    while(invalido);
    strcpy(c.email, auxCARACTER);

    do
    {
        printf("Contrase%ca: ", 164);
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
        auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = strlen(auxCARACTER) == 0;
        if(invalido) puts("Debe llenar el campo");
    }
    while(invalido);
    strcpy(c.contrasenia, auxCARACTER);

    do
    {
        printf("Direccion: ");
        fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
        auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
        invalido = strlen(auxCARACTER) == 0;
        if(invalido) puts("Debe llenar el campo");
    }
    while(invalido);
    strcpy(c.direccion, auxCARACTER);

    if(altaCliente(c) == 1)
    {
        printf("¡Cliente guardado exitosamente!\n");
    }
    else
    {
        printf("Error: No se pudo guardar el cliente.\n");
    }
}

void pantallaModificarCliente()
{
    int auxENTERO, invalido;
    char nuevaDir[150], nuevaPass[50];
    printf("\n--- MODIFICAR CLIENTE ---\n");
    printf("Ingrese el ID del cliente a modificar: ");
    scanf("%d", &auxENTERO);
    getchar();

    if(buscarClientePorID(auxENTERO) == 0)
    {
        puts("Error: El ID no existe o el cliente esta de baja.");
    }
    else
    {
        do
        {
            printf("Ingrese Nueva Direccion: ");
            fgets(nuevaDir, sizeof(nuevaDir), stdin);
            nuevaDir[strcspn(nuevaDir, "\n")] = '\0';
            invalido = strlen(nuevaDir) == 0;
            if(invalido) puts("Debe llenar el campo");
        }
        while(invalido);

        do
        {
            printf("Ingrese Nueva Contrase%ca: ", 164);
            fgets(nuevaPass, sizeof(nuevaPass), stdin);
            nuevaPass[strcspn(nuevaPass, "\n")] = '\0';
            invalido = strlen(nuevaPass) == 0;
            if(invalido) puts("Debe llenar el campo");
        }
        while(invalido);

        if(modificarCliente(auxENTERO, nuevaDir, nuevaPass) == 1)
        {
            printf("¡Cliente modificado con exito!\n");
        }
        else
        {
            printf("Error al intentar modificar.\n");
        }
    }
}

void pantallaBajaCliente()
{
    int auxENTERO;
    printf("\n--- BAJA DE CLIENTE ---\n");
    printf("Ingrese el ID del cliente a dar de baja: ");
    scanf("%d", &auxENTERO);
    getchar();

    if(buscarClientePorID(auxENTERO) == 0)
    {
        puts("Error: El ID no existe o ya esta de baja.");
    }
    else
    {
        if(bajaCliente(auxENTERO) == 1)
        {
            printf("¡Cliente dado de baja exitosamente!\n");
        }
        else
        {
            printf("Error al intentar dar de baja.\n");
        }
    }
}

void gestionClientes()
{
    int opcABM;
    do
    {
        printf("\n--- ABM CLIENTES ---\n");
        printf("1. Alta de Cliente\n");
        printf("2. Modificar Cliente\n");
        printf("3. Baja de Cliente\n");
        printf("0. Volver al menu principal\n");
        printf("Opcion ABM: ");
        scanf("%d", &opcABM);
        getchar();

        switch(opcABM)
        {
        case 1:
            pantallaAltaCliente();
            break;
        case 2:
            pantallaModificarCliente();
            break;
        case 3:
            pantallaBajaCliente();
            break;
        case 0:
            printf("Volviendo...\n");
            break;
        default:
            printf("Opcion invalida.\n");
        }
    }
    while(opcABM != 0);
}

int modificarCliente(int id, char nuevaDireccion[], char nuevaContrasenia[])
{
    FILE *f = abrirArchivo("clientes.dat", "rb+");
    if(f == NULL) return 0;

    Cliente aux;
    int modificado = 0;

    while(leerRegistro(f, &aux, sizeof(Cliente)))
    {
        if(aux.id_cliente == id && aux.activo == 1)
        {
            strcpy(aux.direccion, nuevaDireccion);
            strcpy(aux.contrasenia, nuevaContrasenia);

            fseek(f, -sizeof(Cliente), SEEK_CUR);
            modificado = escribirRegistro(f, &aux, sizeof(Cliente));
            break;
        }
    }
    fclose(f);
    return modificado;
}

int bajaCliente(int id)
{
    FILE *f = abrirArchivo("clientes.dat", "rb+");
    if(f == NULL) return 0;

    Cliente aux;
    int dadoDeBaja = 0;

    while(leerRegistro(f, &aux, sizeof(Cliente)))
    {
        if(aux.id_cliente == id && aux.activo == 1)
        {
            aux.activo = 0;

            fseek(f, -sizeof(Cliente), SEEK_CUR);
            dadoDeBaja = escribirRegistro(f, &aux, sizeof(Cliente));
            break;
        }
    }
    fclose(f);
    return dadoDeBaja;
}


int buscarClientePorID(int id)
{
    FILE *f = abrirArchivo("clientes.dat", "rb");
    if(f == NULL) return 0;

    Cliente aux;
    while(leerRegistro(f, &aux, sizeof(Cliente)))
    {
        if(aux.id_cliente == id && aux.activo == 1)
        {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int guardarCliente(Cliente c)
{
    FILE *f = abrirArchivo("clientes.dat", "ab");
    if(f == NULL) return 0;

    int resultado = escribirRegistro(f, &c, sizeof(Cliente));
    fclose(f);
    return resultado;
}


// --- 2. VALIDACIONES ---

int validarNombre(char nombre[])
{
    int len = strlen(nombre);
    if(len == 0) return 0;

    for(int i = 0; i < len; i++)
    {
        if(!isalpha(nombre[i]) && nombre[i] != ' ')
        {
            return 0;
        }
    }
    return 1;
}

int validarEmail(char email[])
{
    if(strlen(email) == 0) return 0;

    char *arroba = strchr(email, '@');
    char *punto = strrchr(email, '.');

    if(arroba != NULL && punto != NULL && arroba + 1 < punto)
    {
        return 1;
    }
    return 0;
}

int idDisponible(int id)
{
    FILE *f = abrirArchivo("clientes.dat", "rb");
    if(f == NULL) return 1;

    Cliente aux;
    while(leerRegistro(f, &aux, sizeof(Cliente)))
    {
        if(aux.id_cliente == id)
        {
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    return 1;
}

int altaCliente(Cliente nuevo)
{
    nuevo.activo = 1;
    return guardarCliente(nuevo);
}

// --- 3. GESTIÓN DE PEDIDOS ---

int altaPedido(PedidoCliente nuevoPedido, ProductosPedido detalles[], int cantidadProductos)
{
    nuevoPedido.activo = 1;
    nuevoPedido.estado = 1;

    FILE *fPedidos = abrirArchivo("pedidos.dat", "ab");
    if(fPedidos == NULL) return 0;

    int okPedido = escribirRegistro(fPedidos, &nuevoPedido, sizeof(PedidoCliente));
    fclose(fPedidos);

    if(!okPedido)
    {
        printf("Error: No se pudo registrar el pedido.\n");
        return 0;
    }

    FILE *fDetalles = abrirArchivo("detalles_pedido.dat", "ab");
    if(fDetalles == NULL) return 0;

    for(int i = 0; i < cantidadProductos; i++)
    {
        detalles[i].id_pedido = nuevoPedido.id_pedido;
        escribirRegistro(fDetalles, &detalles[i], sizeof(ProductosPedido));
    }
    fclose(fDetalles);
    return 1;
}

int modificarEstadoPedido(int id_pedido, int nuevoEstado)
{
    FILE *f = abrirArchivo("pedidos.dat", "rb+");
    if(f == NULL) return 0;

    PedidoCliente aux;
    int modificado = 0;

    while(leerRegistro(f, &aux, sizeof(PedidoCliente)))
    {
        if(aux.id_pedido == id_pedido && aux.activo == 1)
        {
            aux.estado = nuevoEstado; // Pisamos el estado viejo

            fseek(f, -sizeof(PedidoCliente), SEEK_CUR);
            modificado = escribirRegistro(f, &aux, sizeof(PedidoCliente));
            break;
        }
    }
    fclose(f);
    return modificado;
}

int bajaPedido(int id_pedido)
{
    FILE *f = abrirArchivo("pedidos.dat", "rb+");
    if(f == NULL) return 0;

    PedidoCliente aux;
    int dadoDeBaja = 0;

    while(leerRegistro(f, &aux, sizeof(PedidoCliente)))
    {
        if(aux.id_pedido == id_pedido && aux.activo == 1)
        {
            aux.activo = 0; // Baja lógica

            fseek(f, -sizeof(PedidoCliente), SEEK_CUR);
            dadoDeBaja = escribirRegistro(f, &aux, sizeof(PedidoCliente));
            break;
        }
    }
    fclose(f);
    return dadoDeBaja;
}

void pantallaAltaPedido()
{
    PedidoCliente p;
    ProductosPedido detalles[50]; // Arreglo temporal para guardar hasta 50 platos
    int cantidadProductos = 0;
    int auxENTERO, invalido;
    char auxCARACTER[150];
    char opcionSigue;

    printf("\n--- NUEVO PEDIDO ---\n");

    // 1. Validar Cliente (Clave Foranea)
    do
    {
        printf("ID del Cliente: ");
        scanf("%d", &auxENTERO);
        getchar();
        invalido = buscarClientePorID(auxENTERO) == 0;
        if(invalido) puts("Error: Cliente no encontrado o dado de baja.");
    }
    while(invalido);
    p.id_cliente = auxENTERO;

    // 2. Validar Restaurante (Clave Foranea)
    do
    {
        printf("ID del Restaurante: ");
        scanf("%d", &auxENTERO);
        getchar();
        invalido = buscarRestaurantePorID(auxENTERO) == 0;
        if(invalido) puts("Error: Restaurante no encontrado o inactivo.");
    }
    while(invalido);
    p.id_usuario_restaurante = auxENTERO;

    // 3. Datos del ticket (Cabecera)
    printf("ID del Pedido (Numero de ticket): ");
    scanf("%d", &p.id_pedido);
    getchar();

    printf("Fecha (YYYY-MM-DD): ");
    fgets(p.fecha, sizeof(p.fecha), stdin);
    p.fecha[strcspn(p.fecha, "\n")] = '\0';

    printf("Codigo de envio: ");
    fgets(p.codigo_envio, sizeof(p.codigo_envio), stdin);
    p.codigo_envio[strcspn(p.codigo_envio, "\n")] = '\0';

    printf("Ubicacion de entrega: ");
    fgets(p.ubicacion_entrega, sizeof(p.ubicacion_entrega), stdin);
    p.ubicacion_entrega[strcspn(p.ubicacion_entrega, "\n")] = '\0';

    // 4. Bucle del Menú (Detalles)
    printf("\n--- CARGA DE PLATOS ---\n");
    do
    {
        do
        {
            printf("ID del Plato a agregar: ");
            scanf("%d", &auxENTERO);
            getchar();
            // Validamos que el plato pertenezca al restaurante elegido
            invalido = buscarProductoPorID(p.id_usuario_restaurante, auxENTERO) == 0;
            if(invalido) puts("Error: Ese plato no existe en este restaurante.");
        }
        while(invalido);

        detalles[cantidadProductos].id_producto = auxENTERO;

        printf("Cantidad: ");
        scanf("%d", &detalles[cantidadProductos].cantidad);
        getchar();

        cantidadProductos++;

        printf("Desea agregar otro plato a este pedido? (S/N): ");
        scanf("%c", &opcionSigue);
        getchar();

    }
    while((opcionSigue == 'S' || opcionSigue == 's') && cantidadProductos < 50);

    // 5. El Guardado Final (Llamada a tu función)
    if(altaPedido(p, detalles, cantidadProductos) == 1)
    {
        printf("\n¡Pedido registrado y guardado con exito!\n");
    }
    else
    {
        printf("\nError al guardar el pedido.\n");
    }
}
void pantallaModificarEstadoPedido()
{
    int id_buscado, nuevoEstado, invalido;

    printf("\n--- MODIFICAR ESTADO DE PEDIDO ---\n");
    printf("Ingrese el ID del Pedido (Ticket): ");
    scanf("%d", &id_buscado);
    getchar();

    do
    {
        printf("Nuevo Estado (1=Pendiente, 2=En Viaje, 3=Entregado): ");
        scanf("%d", &nuevoEstado);
        getchar();
        invalido = (nuevoEstado < 1 || nuevoEstado > 3);
        if(invalido) puts("Estado invalido. Por favor ingrese 1, 2 o 3.");
    }
    while(invalido);

    if(modificarEstadoPedido(id_buscado, nuevoEstado) == 1)
    {
        printf("¡Estado del pedido actualizado exitosamente!\n");
    }
    else
    {
        printf("Error: No se encontro el pedido o esta dado de baja.\n");
    }
}

void pantallaBajaPedido()
{
    int id_buscado;

    printf("\n--- CANCELAR / BAJA DE PEDIDO ---\n");
    printf("Ingrese el ID del Pedido a cancelar: ");
    scanf("%d", &id_buscado);
    getchar();

    if(bajaPedido(id_buscado) == 1)
    {
        printf("¡Pedido cancelado (baja logica) exitosamente!\n");
    }
    else
    {
        printf("Error: No se encontro el pedido o ya estaba cancelado.\n");
    }
}

void gestionPedidos()
{
    int opcABM;
    do
    {
        printf("\n--- GESTION DE PEDIDOS ---\n");
        printf("1. Alta de Nuevo Pedido\n");
        printf("2. Modificar Estado de Pedido\n"); // De Pendiente a En Viaje, etc.
        printf("3. Baja/Cancelacion de Pedido\n");
        printf("0. Volver al menu principal\n");
        printf("Opcion ABM: ");
        scanf("%d", &opcABM);
        getchar();

        switch(opcABM)
        {
        case 1:
            pantallaAltaPedido();
            break;
        case 2:
            pantallaModificarEstadoPedido();
            break;
        case 3:
            pantallaBajaPedido();
            break;
        case 0:
            printf("Volviendo...\n");
            break;
        default:
            printf("Opcion invalida.\n");
        }
    }
    while(opcABM != 0);
}

void pantallaListarPedidosCliente()
{
    int id_buscado;
    printf("\n--- LISTAR PEDIDOS POR CLIENTE ---\n");
    printf("Ingrese el ID del Cliente: ");
    scanf("%d", &id_buscado);
    getchar();

    // Validar si el cliente existe...
    if(buscarClientePorID(id_buscado) == 0)
    {
        printf("Error: El cliente no existe o esta de baja.\n");
        return;
    }

    // Acá tu compañera puede meter el while() que lea pedidos.dat
    // y filtre los que coincidan con id_buscado.
    printf("\n[DEBUG] Funcion en construccion. Aca se listaran los pedidos del cliente %d\n", id_buscado);
}

//--- DEBUG ---

void generarInformesTxt()
{
    printf("\n--- GENERACION DE INFORMES ---\n");
    // Acá tu compañera puede armar la lógica de crear el .txt
    printf("\n[DEBUG] Funcion en construccion. Aca se exportaran los datos a texto.\n");
}


void listarClientesDebug()
{
    FILE *f = abrirArchivo("clientes.dat", "rb");
    if(f == NULL)
    {
        printf("El archivo no existe o todavia esta vacio.\n");
        return;
    }

    Cliente aux;
    printf("\n=== DEBUG: TODOS LOS CLIENTES EN LA BASE DE DATOS ===\n");

    while(leerRegistro(f, &aux, sizeof(Cliente)))
    {
        printf("ID Cliente : %d\n", aux.id_cliente);
        printf("Estado     : %s\n", aux.activo == 1 ? "Activo (1)" : "Baja (0)");
        printf("Nombre     : %s\n", aux.nombre);
        printf("Email      : %s\n", aux.email);
        printf("Direccion  : %s\n", aux.direccion);
        printf("Contrasenia: %s\n", aux.contrasenia);
        printf("---------------------------------------------------\n");
    }
    fclose(f);
}


void listarRestaurantesDebug()
{
    FILE *f = abrirArchivo("restaurantes.dat", "rb");
    if (f == NULL)
    {
        printf("El archivo de restaurantes no existe o todavia esta vacio.\n");
        return;
    }

    Restaurante aux;
    printf("\n=== DEBUG: TODOS LOS RESTAURANTES EN LA BASE DE DATOS ===\n");

    while (leerRegistro(f, &aux, sizeof(Restaurante)))
    {
        printf("ID Restaurante: %d\n", aux.id_restaurante);
        printf("Estado        : %s\n", aux.activo == 1 ? "Activo (1)" : "Baja (0)");
        printf("Nombre        : %s\n", aux.nombre);
        printf("Descripcion   : %s\n", aux.descripcion);
        printf("---------------------------------------------------\n");
    }
    fclose(f);
}

void listarProductosDebug()
{
    FILE *f = abrirArchivo("productos.dat", "rb");
    if (f == NULL)
    {
        printf("El archivo de productos no existe o todavia esta vacio.\n");
        return;
    }

    Producto aux;
    printf("\n=== DEBUG: TODOS LOS PLATOS (PRODUCTOS) ===\n");
    while (leerRegistro(f, &aux, sizeof(Producto)))
    {
        printf("ID Plato      : %d\n", aux.id_producto);
        printf("ID Resto Due%co: %d\n", 164, aux.id_usuario_restaurante);
        printf("Nombre        : %s\n", aux.nombre);
        printf("Precio        : $%.2f\n", aux.precio);
        printf("Estado        : %d\n", aux.activo);
        printf("---------------------------------------------------\n");
    }
    fclose(f);
}

void listarPedidosDebug()
{
    FILE *f = abrirArchivo("pedidos.dat", "rb");
    if (f == NULL)
    {
        printf("El archivo de pedidos no existe o todavia esta vacio.\n");
        return;
    }

    PedidoCliente aux;
    printf("\n=== DEBUG: TODOS LOS PEDIDOS (CABECERAS) ===\n");
    while (leerRegistro(f, &aux, sizeof(PedidoCliente)))
    {
        printf("ID Pedido     : %d\n", aux.id_pedido);
        printf("ID Cliente    : %d\n", aux.id_cliente);
        printf("ID Resto      : %d\n", aux.id_usuario_restaurante);
        printf("Fecha         : %s\n", aux.fecha);
        printf("Estado        : %d\n", aux.activo);
        printf("---------------------------------------------------\n");
    }
    fclose(f);
}
