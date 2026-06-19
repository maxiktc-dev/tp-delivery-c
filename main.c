#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "crud.h"
#include "filemanager.h"
#include "estructuras.h"

int main()
{
    int opcion;
    Cliente c;

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
                printf("\n--- ALTA DE CLIENTE ---\n");
                printf("ID: ");
                scanf("%d", &c.id_cliente);
                getchar();

                printf("Nombre: ");
                fgets(c.nombre, sizeof(c.nombre), stdin);
                c.nombre[strcspn(c.nombre, "\n")] = '\0';

                printf("Email: ");
                fgets(c.email, sizeof(c.email), stdin);
                c.email[strcspn(c.email, "\n")] = '\0';

                // Agrega la contraseña (con tu arreglo para la 'ñ')
                printf("Contrase%ca: ", 164);
                fgets(c.contrasenia, sizeof(c.contrasenia), stdin);
                c.contrasenia[strcspn(c.contrasenia, "\n")] = '\0';

                printf("Direccion: ");
                fgets(c.direccion, sizeof(c.direccion), stdin);
                c.direccion[strcspn(c.direccion, "\n")] = '\0';

                // Verifica la validez de los datos
                if(altaCliente(c) == 1)
                {
                    printf("¡Cliente guardado exitosamente en clientes.dat!\n");
                }
                else
                {
                    printf("Error: No se pudo guardar el cliente. Verifique las validaciones.\n");
                }
                break;

            case 2:
//---Funcion temporal para chequear que ande bien la alta de clientes
listarClientesDebug();

                break;
        }

    } while(opcion != 0);

    return 0;
}
