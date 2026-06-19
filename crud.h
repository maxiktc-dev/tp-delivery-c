#ifndef CRUD_H
#define CRUD_H

#include "estructuras.h"

// --- CATEGORÍA 1: CLIENTES ---
void gestionClientes();
void pantallaAltaCliente();
void pantallaModificarCliente();
void pantallaBajaCliente();
void pantallaReactivarCliente();
int buscarClientePorID(int id);
int buscarClientePorIdYContrasenia(int id, char pass[]);
int guardarCliente(Cliente c);
int modificarClienteCompleto(int id, char nuevaDireccion[], char nuevaContrasenia[], char nuevoTelefono[]);
int bajaCliente(int id);
int checkEstadoClienteGlobal(int id);
int reactivarClienteSimple(int id);
void listarClientesDebug();

// --- CATEGORÍA 2: RESTAURANTES ---
void gestionRestaurantes();
void pantallaAltaRestaurante();
void pantallaModificacionRestaurante();
void pantallaBajaRestaurante();
void pantallaReactivarRestaurante();
int buscarRestaurantePorID(int id);
int buscarRestoPorIdYContrasenia(int id, char pass[]);
int guardarRestaurante(Restaurante r);
int modificarRestaurante(int id, char nuevoNombre[], char nuevaDescripcion[]);
int bajaRestaurante(int id);
int checkEstadoRestauranteGlobal(int id);
int reactivarRestauranteSimple(int id);
void listarRestaurantesDebug();

// --- CATEGORÍA 3: PLATOS ---
void gestionPlatosRestoEspecifico(int id_rest);
void menuAltaProducto(int id_rest);
void menuModificarProducto(int id_rest);
void menuBajaProducto(int id_rest);
void menuReactivarProducto(int id_rest);
int buscarProductoPorID(int id_restaurante, int id_producto);
int guardarProducto(Producto p);
int modificarProducto(int id_restaurante, int id_producto, char nuevoNombre[], float nuevoPrecio);
int bajaProducto(int id_restaurante, int id_producto);
int checkEstadoProductoGlobal(int id_restaurante, int id_producto);
int reactivarProductoSimple(int id_restaurante, int id_producto);
int reactivarYModificarProducto(Producto p);
void listarProductosDebug();

// --- CATEGORÍA 4: PEDIDOS ---
void pantallaAltaPedidoCliente(int id_logueado);
void pantallaListarPedidosUnicoCliente(int id_logueado);
void gestionPedidosRestoEspecifico(int id_rest);
int altaPedido(PedidoCliente nuevoPedido, ProductosPedido detalles[], int cantidadProductos);
int modificarEstadoPedido(int id_pedido, int nuevoEstado);
int bajaPedido(int id_pedido);
void listarPedidosDebug();

// --- CATEGORÍA 5: VALIDACIONES ---
int validarTelefono(char telefono[]);
int validarNombre(char nombre[]);
int validarEmail(char email[]);
int validarFecha(char fecha[]);

#endif // CRUD_H
