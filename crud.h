#ifndef CRUD_H
#define CRUD_H

#include "estructuras.h"

int buscarUsuarioPorID(int id);

int guardarUsuario(Usuario u);

int altaUsuario(Usuario nuevo);

void listarUsuarios();

int validarUsuario(Usuario u);

int idDisponible(int id);

#endif
 // CRUD_H_INCLUDED
