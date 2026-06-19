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
    getchar();

    printf("Ingrese su Contrase%ca: ", 164);
    fgets(pass_ingresada, sizeof(pass_ingresada), stdin);
    pass_ingresada[strcspn(pass_ingresada, "\n")] = '\0';

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
        printf("3. Modificar mis Datos Personales\n"); // ¡AGREGADO!
        printf("4. Dar de Baja mi Cuenta\n");           // ¡AGREGADO!
        printf("0. Cerrar Sesion\n");
        printf("Opcion: ");
        scanf("%d", &opc);
        getchar();

        switch(opc) {
            case 1:
                pantallaAltaPedidoCliente(id_clie);
                break;
            case 2:
                pantallaListarPedidosUnicoCliente(id_clie);
                break;
            case 3:
                // Modifica los datos inyectando directamente el ID logueado
                if(buscarClientePorID(id_clie)) {
                    int invalido;
                    char dir[150], pass[50], tel[20];
                    printf("\n--- ACTUALIZAR MIS DATOS ---\n");
                    do {
                        printf("Nueva Direccion: "); fgets(dir, 150, stdin); dir[strcspn(dir, "\n")] = '\0';
                        invalido = strlen(dir) == 0;
                    } while(invalido);

                    do {
                        printf("Nueva Contrase%ca: ", 164); fgets(pass, 50, stdin); pass[strcspn(pass, "\n")] = '\0';
                        invalido = strlen(pass) == 0;
                    } while(invalido);

                    do {
                        printf("Nuevo Telefono: "); fgets(tel, 20, stdin); tel[strcspn(tel, "\n")] = '\0';
                        invalido = validarTelefono(tel) == 0;
                    } while(invalido);

                    modificarClienteCompleto(id_clie, dir, pass, tel);
                    printf("¡Tus datos fueron actualizados correctamente!\n");
                }
                break;
            case 4:
                printf("\n--- DAR DE BAJA CUENTA ---\n");
                printf("¿Esta seguro de que desea desactivar su cuenta? (S/N): ");
                char confirma;
                scanf("%c", &confirma);
                getchar();
                if(confirma == 'S' || confirma == 's') {
                    bajaCliente(id_clie);
                    printf("Tu cuenta fue dada de baja. Saliendo...\n");
                    opc = 0; // Fuerza el cierre de sesión inmediato
                }
                break;
            case 0:
                printf("Cerrando sesion de cliente...\n");
                break;
            default:
                printf("Opcion invalida.\n");
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
        printf("4. Modificar Datos de mi Local\n");
        printf("5. Dar de Baja el Restaurante\n");
        printf("0. Cerrar Sesion\n");
        printf("Opcion: ");
        scanf("%d", &opc);
        getchar();

        switch(opc) {
            case 1:
                gestionPlatosRestoEspecifico(id_rest);
                break;
            case 2:
                gestionPedidosRestoEspecifico(id_rest);
                break;
            case 3:
                generarInformesTxt();
                break;
            case 4:
                if(buscarRestaurantePorID(id_rest)) {
                    int invalido;
                    char nom[50], dir[150], desc[150]; // Agregada variable 'desc'
                    printf("\n--- ACTUALIZAR DATOS DE MI RESTAURANTE ---\n");

                    do {
                        printf("Nuevo Nombre Comercial: ");
                        fgets(nom, 50, stdin); nom[strcspn(nom, "\n")] = '\0';
                        invalido = validarNombre(nom) == 0;
                    } while(invalido);

                    do {
                        printf("Nueva Direccion: ");
                        fgets(dir, 150, stdin); dir[strcspn(dir, "\n")] = '\0';
                        invalido = strlen(dir) == 0;
                    } while(invalido);

                    // ¡NUEVO BUCLE: Captura la nueva descripción del local!
                    do {
                        printf("Nueva Descripcion / Rubro: ");
                        fgets(desc, 150, stdin); desc[strcspn(desc, "\n")] = '\0';
                        invalido = strlen(desc) == 0;
                    } while(invalido);

                    // Pasamos 'desc' en lugar de 'dir' en el tercer parámetro para que crud.c lo actualice
                    modificarRestaurante(id_rest, nom, desc);
                    printf("¡Datos del local modificados con exito!\n");
                }
                break;
            case 5:
                printf("\n--- BAJA COMERCIAL DEL RESTAURANTE ---\n");
                printf("¿Esta seguro de dar de baja el local? (Se suspenderan todos los platos en cascada) (S/N): ");
                char confirmaResto;
                scanf("%c", &confirmaResto);
                getchar();
                if(confirmaResto == 'S' || confirmaResto == 's') {
                    bajaRestaurante(id_rest);
                    printf("El restaurante y su menu fueron desactivados. Saliendo...\n");
                    opc = 0;
                }
                break;
            case 0:
                printf("Cerrando sesion corporativa...\n");
                break;
            default:
                printf("Opcion invalida.\n");
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
        printf("2. Registrar un Nuevo Restaurante (Local)\n");
        printf("3. Ingresar como Cliente\n");
        printf("4. Ingresar como Restaurante\n");
        printf("0. Salir\n");

        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch(opcion)
        {
            case 1: pantallaAltaCliente(); break;
            case 2: pantallaAltaRestaurante(); break;
            case 3: sesionCliente(); break;
            case 4: sesionRestaurante(); break;
            case 0: printf("Saliendo del sistema de forma segura...\n"); break;
            default: printf("Opcion invalida. Intente nuevamente.\n");
        }

    } while(opcion != 0);

    return 0;
}
