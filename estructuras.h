#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef struct
{
    int id_cliente;        // CP
    char direccion[150];
    char contrasenia[50];
    char email[100];
    char nombre[50];
    char telefono[20];
    int activo;            // Para manejar bajas lógicas
} Cliente;

typedef struct
{
    int id_restaurante;          // CP
    char nombre[50];             //
    char direccion[150];         // Dirección general del local
    char descripcion[150];       // Rubro
    char contrasenia[50];        // ¡AGREGADA PARA EL INGRESO SEGURO!
    float calificacion;          // Calificación promedio
    int activo;                  // 1 = Activo, 0 = Baja lógica
} Restaurante;

typedef struct
{
    int id_pedido;               // CP
    int id_cliente;              // CF
    int id_usuario_restaurante;  // CF
    char fecha[11];              // YYYY-MM-DD
    char codigo_envio[20];
    char ubicacion_entrega[100]; // Dirección específica de entrega del pedido
    float total;
    int estado;                  // 1 = Pendiente, 2 = En viaje, 3 = Entregado
    int activo;                  // 1 = Activo, 0 = Cancelado
} PedidoCliente;

typedef struct
{
    int id_producto;
    int id_usuario_restaurante;
    char nombre[50];
    char descripcion[150];
    float precio;
    int estado;
    int activo;
} Producto;

typedef struct
{
    int id_detalle;
    int id_pedido;
    int id_producto;
    int cantidad;
    float subtotal;
} ProductosPedido;

typedef struct {
    int id_calificacion;
    int id_cliente;
    int id_restaurante;
    int id_pedido;
    int estrellas;
    char comentario[150];
    char fecha[11];
    char comentario_respuesta[150];
    char fecha_respuesta[11];
} Calificacion;

#endif // ESTRUCTURAS_H
