#ifndef CRUD_H
#define CRUD_H

#include "estructuras.h"

// ==========================================
// --- 1. ABM DE PLATOS (PRODUCTOS) ---
// ==========================================
void gestionPlatos();
void menuAltaProducto(int id_rest);
void menuModificarProducto(int id_rest);
void menuBajaProducto(int id_rest);
int buscarProductoPorID(int id_restaurante, int id_producto);
int guardarProducto(Producto p);
int modificarProducto(int id_restaurante, int id_producto, char nuevoNombre[], float nuevoPrecio);
int bajaProducto(int id_restaurante, int id_producto);
void listarProductosDebug();

// ==========================================
// --- 2. ABM DE RESTAURANTES ---
// ==========================================
void gestionRestaurantes();
void pantallaAltaRestaurante();
void pantallaModificacionRestaurante();
void pantallaBajaRestaurante();
void listarRestaurantesDebug();
int guardarRestaurante(Restaurante r);
int buscarRestaurantePorID(int id);
int modificarRestaurante(int id, char nuevoNombre[], char nuevaDescripcion[]);
int bajaRestaurante(int id);

// ==========================================
// --- 3. ABM Y VALIDACIONES DE CLIENTES ---
// ==========================================
void gestionClientes();
void pantallaAltaCliente();
void pantallaModificarCliente();
void pantallaBajaCliente();
void listarClientes();
void listarClientesDebug();
int validarNombre(char nombre[]);
int validarEmail(char email[]);
int modificarCliente(int id, char nuevaDireccion[], char nuevaContrasenia[]);
int bajaCliente(int id);
int buscarClientePorID(int id);
int guardarCliente(Cliente u);
int altaCliente(Cliente nuevo);
int validarCliente(Cliente u);
int idDisponible(int id);

// ==========================================
// --- 4. ABM DE PEDIDOS ---
// ==========================================
void gestionPedidos();
void pantallaAltaPedido();
void pantallaModificarEstadoPedido();
void pantallaBajaPedido();
int altaPedido(PedidoCliente nuevoPedido, ProductosPedido detalles[], int cantidadProductos);
int modificarEstadoPedido(int id_pedido, int nuevoEstado);
int bajaPedido(int id_pedido);
void listarPedidosDebug();
#endif // CRUD_H
