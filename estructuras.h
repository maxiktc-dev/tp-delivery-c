#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef struct
{
    int id_cliente;        // CP
    char direccion[150];
    char contrasenia[50];
    char email[100];
    char nombre[50];
    int activo;            // Para manejar bajas lógicas
} Cliente;

typedef struct
{
    int id_restaurante;          // Clave Primaria
    char nombre[50];
    char descripcion[150];       // Ej: "Pizzería", "Comida Vegana"
    int activo;                  // 1 = Activo, 0 = Baja lógica
} Restaurante;

typedef struct
{
    int id_pedido;  // CP Id_pedido
    int id_cliente; // CF id_usuario_cliente
    int id_usuario_restaurante; // CF id_usuario_restaurante
    char fecha[11]; // Fecha (EJ: "2026-06-18")
    char codigo_envio[20]; // Codigo_envio
    char ubicacion_entrega[100]; // Ubicacion_entrega
    int estado; // el estado tipo 1 = Pendiente, 2 = En viaje, 3 = Entregado
    int activo; // 1 = Activo, 0 = Dado de baja (lo mismo que en el usuario)

} PedidoCliente;

typedef struct
{
    int id_producto; // CP Id_producto
    int id_usuario_restaurante; // CF Id_restaurante
    char nombre[50]; //Nombre del producto
    char descripcion[150]; // Descripción del producto
    float precio; // Precio del producto
    int estado; // Estado del producto (1 = Disponible, 0 = Agotado);
    int activo; // Para la baja lógica del producto.
} Producto;

typedef struct
{
    int id_pedido;
    int id_producto;
    int cantidad;
} ProductosPedido;
#endif// ESTRUCTURAS_H_INCLUDED
