#ifndef CRUD_H
#define CRUD_H

#include "estructuras.h"

//--- ABM DE CLIENTES ---

int buscarClientePorID(int id);

int guardarCliente(Cliente u);

int altaCliente(Cliente nuevo);

void listarClientes();

int validarCliente(Cliente u);

int idDisponible(int id);

void listarClientesDebug();
//--- ABM DE PEDIDOS ---

int altaPedido(PedidoCliente nuevoPedido, ProductosPedido detalles[], int cantidadProductos);
#endif
 // CRUD_H_INCLUDED
