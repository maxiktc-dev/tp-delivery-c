#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "estructuras.h"
#include "crud.h"
#include "filemanager.h"


int main()
{
    int opcion;
    Cliente c;
    int auxENTERO = 0;
    char auxCARACTER[150];
    bool invalido = false;

    do
    {
        printf("\n");
        printf("===== DELIVERY =====\n");
        printf("1. ABM Cliente\n");
        printf("2. ABM de Restaurantes\n");
        printf("3. ABM de Platos\n");
        printf("4. Registrar nuevo Pedido\n");
        printf("5. Actualizar estado de Pedido\n");
        printf("6. Listar Pedidos por Cliente\n");
        printf("7. Informes en archivo de texto\n");
        printf("0. Salir\n");

        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

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

            case 9:
                listarClientesDebug();
                break;
            case 10:
                listarRestaurantesDebug();
                break;
        }

    } while(opcion != 0);

    return 0;
}
