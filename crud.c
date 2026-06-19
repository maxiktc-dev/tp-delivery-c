
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "crud.h"
#include "estructuras.h"
#include "filemanager.h"

// --- 1. ABM Y LÓGICA DE CLIENTES ---


void gestionClientes() {
    int opcABM;
    Cliente c;
    int auxENTERO = 0;
    char auxCARACTER[150];
    int invalido = 0;

    printf("\n--- ABM CLIENTES ---\n");
    printf("1. Alta de Cliente\n");
    printf("2. Modificar Cliente\n");
    printf("3. Baja de Cliente\n");
    printf("Opcion ABM: ");
    scanf("%d", &opcABM);
    getchar();

    if(opcABM == 1) {
        printf("\n--- ALTA DE CLIENTE ---\n");
        do {
            printf("ID: ");
            scanf("%d", &auxENTERO);
            getchar();
            invalido = buscarClientePorID(auxENTERO) == 1;
            if(invalido) puts("Ese ID ya existe, ingrese otro!");
        } while(invalido);
        c.id_cliente = auxENTERO;

        do {
            printf("Nombre: ");
            fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
            auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
            invalido = validarNombre(auxCARACTER) == 0;
            if(invalido) puts("Nombre invalido, intente devuelta");
        } while(invalido);
        strcpy(c.nombre, auxCARACTER);

        do {
            printf("Email: ");
            fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
            auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
            invalido = validarEmail(auxCARACTER) == 0;
            if(invalido) puts("El email es invalido, intente denuevo");
        } while(invalido);
        strcpy(c.email, auxCARACTER);

        do {
            printf("Contrase%ca: ", 164);
            fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
            auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
            invalido = strlen(auxCARACTER) == 0;
            if(invalido) puts("Debe llenar el campo");
        } while(invalido);
        strcpy(c.contrasenia, auxCARACTER);

        do {
            printf("Direccion: ");
            fgets(auxCARACTER, sizeof(auxCARACTER), stdin);
            auxCARACTER[strcspn(auxCARACTER, "\n")] = '\0';
            invalido = strlen(auxCARACTER) == 0;
            if(invalido) puts("Debe llenar el campo");
        } while(invalido);
        strcpy(c.direccion, auxCARACTER);

        if(altaCliente(c) == 1) {
            printf("¡Cliente guardado exitosamente!\n");
        } else {
            printf("Error: No se pudo guardar el cliente.\n");
        }
    }
    else if(opcABM == 2) {
        char nuevaDir[150], nuevaPass[50];
        printf("\n--- MODIFICAR CLIENTE ---\n");
        printf("Ingrese el ID del cliente a modificar: ");
        scanf("%d", &auxENTERO);
        getchar();

        if(buscarClientePorID(auxENTERO) == 0) {
            puts("Error: El ID no existe o el cliente esta de baja.");
        } else {
            do {
                printf("Ingrese Nueva Direccion: ");
                fgets(nuevaDir, sizeof(nuevaDir), stdin);
                nuevaDir[strcspn(nuevaDir, "\n")] = '\0';
                invalido = strlen(nuevaDir) == 0;
                if(invalido) puts("Debe llenar el campo");
            } while(invalido);

            do {
                printf("Ingrese Nueva Contrase%ca: ", 164);
                fgets(nuevaPass, sizeof(nuevaPass), stdin);
                nuevaPass[strcspn(nuevaPass, "\n")] = '\0';
                invalido = strlen(nuevaPass) == 0;
                if(invalido) puts("Debe llenar el campo");
            } while(invalido);

            if(modificarCliente(auxENTERO, nuevaDir, nuevaPass) == 1) {
                printf("¡Cliente modificado con exito!\n");
            } else {
                printf("Error al intentar modificar.\n");
            }
        }
    }
    else if(opcABM == 3) {
        printf("\n--- BAJA DE CLIENTE ---\n");
        printf("Ingrese el ID del cliente a dar de baja: ");
        scanf("%d", &auxENTERO);
        getchar();

        if(buscarClientePorID(auxENTERO) == 0) {
            puts("Error: El ID no existe o ya esta de baja.");
        } else {
            if(bajaCliente(auxENTERO) == 1) {
                printf("¡Cliente dado de baja exitosamente!\n");
            } else {
                printf("Error al intentar dar de baja.\n");
            }
        }
    }
}




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
    // 1. Verifica largo mayor a 0
    if(strlen(email) == 0) return 0;

    char *arroba = strchr(email, '@');  // Busca el PRIMER arroba
    char *punto = strrchr(email, '.');   // Busca el ÚLTIMO punto (Cambiado a strrchr)

    /* Verifica que existan, que la @ esté antes que el punto,
       y que no estén pegados (@.) */
    if(arroba != NULL && punto != NULL && arroba + 1 < punto) {
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


int altaCliente(Cliente nuevo) {
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


int modificarCliente(int id, char nuevaDireccion[], char nuevaContrasenia[]) {
    // Abrimos en modo rb+ para poder leer y escribir el mismo archivo
    FILE *f = abrirArchivo("clientes.dat", "rb+");
    if(f == NULL) return 0;

    Cliente aux;
    int modificado = 0;

    // Recorremos buscando el registro
    while(leerRegistro(f, &aux, sizeof(Cliente))) {
        if(aux.id_cliente == id && aux.activo == 1) {
            // Modificamos los campos permitidos
            strcpy(aux.direccion, nuevaDireccion);
            strcpy(aux.contrasenia, nuevaContrasenia);

            // Volvemos el cursor hacia atrás el tamaño de un registro
            fseek(f, -sizeof(Cliente), SEEK_CUR);

            // Sobreescribimos el registro modificado
            modificado = escribirRegistro(f, &aux, sizeof(Cliente));
            break; // Salimos del bucle ya que el ID es único
        }
    }

    fclose(f);
    return modificado; // Devuelve 1 si se logró, 0 si no
}

int bajaCliente(int id) {
    FILE *f = abrirArchivo("clientes.dat", "rb+");
    if(f == NULL) return 0;

    Cliente aux;
    int dadoDeBaja = 0;

    while(leerRegistro(f, &aux, sizeof(Cliente))) {
        if(aux.id_cliente == id && aux.activo == 1) {
            aux.activo = 0;

            fseek(f, -sizeof(Cliente), SEEK_CUR);

            dadoDeBaja = escribirRegistro(f, &aux, sizeof(Cliente));

            break;
        }
    }

    fclose(f);
    return dadoDeBaja;
}
