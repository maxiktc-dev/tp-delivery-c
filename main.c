#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "estructuras.h"
#include "crud.h"
#include "filemanager.h"
#include "informes.h"

void sesionCliente() {
    int id_clie;
    char pass_ingresada[50];

    printf("\n--- LOGIN CLIENTE ---\n");
    printf("Ingrese su ID de Cliente: ");
    scanf("%d", &id_clie);
    getchar(); // Limpia el buffer

    printf("Ingrese su Contrase%ca: ", 164);
    fgets(pass_ingresada, sizeof(pass_ingresada), stdin);
    pass_ingresada[strcspn(pass_ingresada, "\n")] = '\0';

    // Verificación real contra el archivo binario de clientes
    if(buscarClientePorIdYContrasenia(id_clie, pass_ingresada) == 0) {
        printf("Error: ID o contrasenia incorrectos, o cuenta inactiva.\n");
        return;
    }

    printf("\n¡Inicio de sesion exitoso! Bienvenido de nuevo.\n");

    int opc;
    do {
        printf("\n===== MENU DE CLIENTE =====\n");
        printf("1. Registrar nuevo Pedido\n");
        printf("2. Ver mi Historial de Pedidos\n");
        printf("0. Cerrar Sesion\n");
        printf("Opcion: ");
        scanf("%d", &opc);
        getchar();

        switch(opc) {
            case 1: pantallaAltaPedidoCliente(id_clie); break;
            case 2: pantallaListarPedidosUnicoCliente(id_clie); break;
            case 0: printf("Cerrando sesion de cliente...\n"); break;
            default: printf("Opcion invalida.\n");
        }
    } while(opc != 0);
}

void sesionRestaurante() {
    int id_rest;
    char pass_ingresada[50];

    printf("\n--- LOGIN RESTAURANTE ---\n");
    printf("Ingrese el ID de su Restaurante: ");
    scanf("%d", &id_rest);
    getchar(); // Limpia el buffer

    printf("Ingrese su Contrase%ca Corporativa: ", 164);
    fgets(pass_ingresada, sizeof(pass_ingresada), stdin);
    pass_ingresada[strcspn(pass_ingresada, "\n")] = '\0';

    // Verificación real contra el archivo binario de restaurantes
    if(buscarRestoPorIdYContrasenia(id_rest, pass_ingresada) == 0) {
        printf("Error: Credenciales incorrectas o local inactivo.\n");
        return;
    }

    printf("\n¡Autenticacion corporativa exitosa! Portal habilitado.\n");

    int opc;
    do {
        printf("\n===== PORTAL RESTAURANTE =====\n");
        printf("1. Gestionar Menu (ABM Platos)\n");
        printf("2. Gestionar Pedidos Recibidos (Ver/Cambiar Estado)\n");
        printf("3. Modulo de Informes y Reportes (.TXT)\n");
        printf("0. Cerrar Sesion\n");
        printf("Opcion: ");
        scanf("%d", &opc);
        getchar();

        switch(opc) {
            case 1: gestionPlatosRestoEspecifico(id_rest); break;
            case 2: gestionPedidosRestoEspecifico(id_rest); break;
            case 3: generarInformesTxt(); break;
            case 0: printf("Cerrando sesion corporativa...\n"); break;
            default: printf("Opcion invalida.\n");
        }
    } while(opc != 0);
}

int main()
{
    int opcion;

    do
    {
        printf("\n===== SISTEMA DE DELIVERY =====\n");
        printf("1. Registrarse como Nuevo Usuario (Cliente)\n");
        printf("2. Registrar un Nuevo Restaurante (Local)\n"); // ¡AGREGADO AQUÍ!
        printf("3. Ingresar como Cliente\n");
        printf("4. Ingresar como Restaurante\n");
        printf("0. Salir\n");

        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar(); // Limpia el buffer

        switch(opcion)
        {
            case 1:
                pantallaAltaCliente();
                break;
            case 2:
                pantallaAltaRestaurante(); // Llama directo a la carga con tus validaciones estrictas
                break;
            case 3:
                sesionCliente();
                break;
            case 4:
                sesionRestaurante();
                break;
            case 0:
                printf("Saliendo del sistema de forma segura...\n");
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }

    } while(opcion != 0);

    return 0;
}
