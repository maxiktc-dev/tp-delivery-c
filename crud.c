
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "crud.h"
#include "estructuras.h"
#include "filemanager.h"

// --- 1. ABM Y LÓGICA DE CLIENTES ---

int buscarClientePorID(int id) {
    FILE *f = abrirArchivo("clientes.dat", "rb");
    if(f == NULL) return 0;

    Cliente aux;
    while(leerRegistro(f, &aux, sizeof(Cliente))) {
/*
*Esto guarda cada estructura del archivo en el auxiliar, y despues compara si
*el id es igual y el estado es Activo, devuelve 1, sino devuelve 0
*/
        if(aux.id_cliente == id && aux.activo == 1) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

int guardarCliente(Cliente c) {
    FILE *f = abrirArchivo("clientes.dat", "ab");
    if(f == NULL) return 0;
    /*esto lo que hace es guardar la estructura en el archivo de clientes, al mismo tiempo
    *te devuelve cuando termina cuantas estructuras guardo, lo que permite saber si guardo 1
    *con exito o si hubo algun error
    */
    int resultado = escribirRegistro(f, &c, sizeof(Cliente));

    fclose(f);
    return resultado;
}

void listarClientes() {
    FILE *f = abrirArchivo("clientes.dat", "rb");
    if(f == NULL) {
        printf("No hay clientes registrados o no se pudo abrir el archivo.\n");
        return;
    }

    Cliente aux;
    printf("\n--- LISTA DE CLIENTES ---\n");
    while(leerRegistro(f, &aux, sizeof(Cliente))) {
        if(aux.activo == 1) {
            printf("ID: %d | Nombre: %s\n", aux.id_cliente, aux.nombre);
        }
    }

    fclose(f);
}

// --- 2. VALIDACIONES ---

int validarNombre(char nombre[]) {

//verifica largo mayor a 0 en la string nombre
    int len = strlen(nombre);
    if(len == 0) return 0;
//si alguno de los caracteres, no es una letra o no es un espacio, devuelve invalido
    for(int i = 0; i < len; i++) {
        if(!isalpha(nombre[i]) && nombre[i] != ' ') {
            return 0;
        }
    }
//sino devuelve valido
    return 1;
}

int validarEmail(char email[]) {

//verifica largo mayor a 0 en la string email
    if(strlen(email) == 0) return 0;

    char *arroba = strchr(email, '@');
    char *punto = strchr(email, '.');

/*verifica que como minimo tenga un arroba, un punto y
*que el arroba este antes que el punto
*/
    if(arroba != NULL && punto != NULL && arroba < punto) {
        return 1;
    }
    return 0;
}

int idDisponible(int id) {
    FILE *f = abrirArchivo("clientes.dat", "rb");
    if(f == NULL) return 1;
//si el archivo ni existe devuelve que el id esta disponible

    Cliente aux;
    while(leerRegistro(f, &aux, sizeof(Cliente))) {
        if(aux.id_cliente == id) {
            fclose(f);
            return 0;
/*si en algun momento del while el id es encontrado cierra el archivo y devuelve
*basicamente que el id no esta disponible
*/
        }
    }

//cierra el archivo y devuelve que el id esta disponible
    fclose(f);
    return 1;
}

int validarCliente(Cliente c) {
    //verifica que el id del cliente sea mayor a 0, y a su vez que este disponible
    if(c.id_cliente <= 0 || !idDisponible(c.id_cliente)) return 0;

    if(!validarNombre(c.nombre)) return 0;

    if(!validarEmail(c.email)) return 0;

    // Agregamos seguridad extra para que no vengan campos vacíos
    if(strlen(c.direccion) == 0) return 0;
    if(strlen(c.contrasenia) == 0) return 0;

    return 1;
}

int altaCliente(Cliente nuevo) {
    if(!validarCliente(nuevo)) {
        printf("Error: Datos invalidos o el ID ya existe.\n");
        return 0;
    }

    //si todo sale bien pasa el estado del cliente a activo y guarda los datos
    nuevo.activo = 1;
    return guardarCliente(nuevo);
}

// --- 3. GESTIÓN DE PEDIDOS ---

int altaPedido(PedidoCliente nuevoPedido, ProductosPedido detalles[], int cantidadProductos) {
    // 1. Forzamos los valores iniciales de seguridad
    nuevoPedido.activo = 1;
    nuevoPedido.estado = 1; // 1 = Pendiente

    // 2. Abrimos y guardamos en pedidos.dat (La Cabecera)
    FILE *fPedidos = abrirArchivo("pedidos.dat", "ab");
    if(fPedidos == NULL) return 0;

    /*guarda basicamente si se guardo 1 pedido con exito, si el numero es distinto
    *de 1 algo fallo
    */
    int okPedido = escribirRegistro(fPedidos, &nuevoPedido, sizeof(PedidoCliente));
    fclose(fPedidos);

    // Si falló el guardado del pedido, cortamos acá
    if(!okPedido) {
        printf("Error: No se pudo registrar el pedido.\n");
        return 0;
    }

    // 3. Abrimos y guardamos en detalles_pedido.dat (El Detalle)
    FILE *fDetalles = abrirArchivo("detalles_pedido.dat", "ab");
    if(fDetalles == NULL) return 0;

    // Iteramos el array de productos y los guardamos uno por uno
    for(int i = 0; i < cantidadProductos; i++) {
        detalles[i].id_pedido = nuevoPedido.id_pedido;
        escribirRegistro(fDetalles, &detalles[i], sizeof(ProductosPedido));
    }

    fclose(fDetalles);
    return 1;
}
void listarClientesDebug() {
    FILE *f = abrirArchivo("clientes.dat", "rb");
    if(f == NULL) {
        printf("El archivo no existe o todavia esta vacio.\n");
        return;
    }

    Cliente aux;
    printf("\n=== DEBUG: TODOS LOS CLIENTES EN LA BASE DE DATOS ===\n");

    // Leemos todo el archivo de principio a fin
    while(leerRegistro(f, &aux, sizeof(Cliente))) {
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
