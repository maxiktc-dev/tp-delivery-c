#include <stdio.h>
#include <string.h>
#include "crud.h"
#include <ctype.h>


int buscarUsuarioPorID(int id)
{
    FILE *f;

    Usuario aux;

    f=fopen("usuarios.dat","rb");

    if(f==NULL)
        return 0;

    while(fread(&aux,sizeof(Usuario),1,f)==1)
    {
        if(aux.idUsuario==id &&
           aux.activo==1)
        {
            fclose(f);

            return 1;
        }
    }

    fclose(f);

    return 0;
}

int guardarUsuario(Usuario u)
{
    FILE *f;

    f=fopen("usuarios.dat","ab");

    if(f==NULL)
        return 0;

    fwrite(&u,
           sizeof(Usuario),
           1,
           f);

    fclose(f);

    return 1;
}

void listarUsuarios()
{
    FILE *f;

    Usuario aux;

    f=fopen("usuarios.dat","rb");

    if(f==NULL)
        return;

    while(fread(&aux,sizeof(Usuario),1,f)==1)
    {
        if(aux.activo)
        {
            printf("%d %s\n",
                   aux.idUsuario,
                   aux.nombre);
        }
    }

    fclose(f);
}


int validarNombre(char nombre[])
{
    int len = strlen(nombre);

    //verifica que no este vacio.

    if(len == 0){
        return 0;
    }

    //si el caracter no es una letra y tampoco es un espacio devuelve invalido.

    for(int i = 0;i < len; i++){
        if(!isalpha(nombre[i]) && nombre[i]!=' '){
            return 0;
        }
    }
    //si cumple con todo devuelve valido.
    return 1;
}

int validarEmail(char email[])
{
    //verifico que no este vacio
    if(strlen(email)==0){
        return 0;
    }
    //busca el arroba y el punto
    char *arroba = strchr(email, '@');
    char *punto = strchr(email, '.');

    //valido que existan ambos y a su vez que el arroba este antes que el punto
    if(arroba !=NULL && punto !=NULL && arroba<punto){
        return 1;
    }

    //si le falta el arroba o punto o si estan al reves devuelve invalido.
    return 0;
}

int idDisponible(int id)
{
    FILE *f;

    Usuario aux;

    f=fopen("usuarios.dat","rb");

    if(f==NULL)
        return 1;

    while(fread(&aux,sizeof(Usuario),1,f)==1)
    {
        if(aux.idUsuario==id)
        {
            fclose(f);

            return 0;
        }
    }

    fclose(f);

    return 1;
}

int validarUsuario(Usuario u)
{
    if(u.idUsuario <=0 || !idDisponible(u.idUsuario)){
        return 0;
    }
    if(!validarNombre(u.nombre))
        return 0;

    if(!validarEmail(u.email))
        return 0;

    return 1;
}

int altaUsuario(Usuario nuevo)
{
    if(!validarUsuario(nuevo))
        return 0;

    return guardarUsuario(nuevo);
}

