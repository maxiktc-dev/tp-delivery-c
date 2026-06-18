#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "crud.h"
#include "filemanager.h"
#include "estructuras.h"
int main()
{
    int opcion;

    Usuario u;

    do
    {
        printf("\n");
        printf("===== DELIVERY =====\n");
        printf("1. Alta Usuario\n");
        printf("2. Listar Usuarios\n");
        printf("0. Salir\n");

        printf("Opcion: ");
        scanf("%d",&opcion);

        getchar();

        switch(opcion)
        {
            case 1:

                printf("ID: ");
                scanf("%d",&u.idUsuario);

                getchar();

                printf("Nombre: ");

                fgets(u.nombre,sizeof(u.nombre),stdin);

                u.nombre[strcspn(u.nombre,"\n")] = '\0';

                printf("Email: ");

                fgets(u.email,sizeof(u.email),stdin);

                u.email[strcspn(u.email,"\n")] = '\0';

                printf("Direccion: ");

                fgets(u.direccion,sizeof(u.direccion),stdin);

                u.direccion[strcspn(u.direccion,"\n")] = '\0';

                printf("Telefono: ");

                fgets(u.telefono,sizeof(u.telefono),stdin);

                u.telefono[strcspn(u.telefono,"\n")] = '\0';

                u.activo=1;

                if(altaUsuario(u)==1)
                {
                    printf("Usuario guardado\n");
                }

                else
                {
                    printf("Error\n");
                }

                break;

            case 2:

                listarUsuarios();

                break;
        }

    }while(opcion!=0);

    return 0;
}
