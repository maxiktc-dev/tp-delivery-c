#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "estructuras.h"
#include "crud.h"
#include "filemanager.h"

int main()
{
    int opcion;

    do
    {
        printf("\n");
        printf("===== SISTEMA DE DELIVERY =====\n");
        printf("1. ABM de Clientes\n");
        printf("2. ABM de Restaurantes\n");
        printf("3. ABM de Platos (Menu)\n");
        printf("4. ABM de Pedidos\n");
        printf("5. Listar Pedidos por Cliente\n");
        printf("6. Informes en archivo de texto\n");
        printf("0. Salir\n");

        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar(); // Limpia el buffer

        switch(opcion)
        {
            case 1:
                gestionClientes();
                break;
            case 2:
                gestionRestaurantes();
                break;
            case 3:
                gestionPlatos();
                break;
            case 4:
                gestionPedidos();
                break;
            case 5:
                pantallaListarPedidosCliente();
                break;
            case 6:
                generarInformesTxt();
                break;
            case 0:
                printf("Saliendo del sistema...\n");
                break;

            // --- ZONA OCULTA DE DEBUG PARA LOS DESARROLLADORES ---
            case 9:
                listarClientesDebug();
                break;
            case 10:
                listarRestaurantesDebug();
                break;
            case 11:
                listarProductosDebug();
                break;
            case 12:
                listarPedidosDebug();
                break;

            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }

    } while(opcion != 0);

    return 0;
}
