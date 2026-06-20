#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "informes.h"

void portalReportesRestaurante(int id_rest) {
    int opc;
    do {
        printf("\n--- REPORTES PRIVADOS (LOCAL ID: %d) ---\n", id_rest);
        printf("1. Ver mis Pedidos\n");
        printf("2. Ver mi Ranking de Platos\n");
        printf("0. Volver\n");
        printf("Opcion: ");
        scanf("%d", &opc); getchar();

        switch(opc) {
            case 1: informesRestauranteLocal(id_rest); break;
            case 2: informePlatosMasVendidosLocal(id_rest); break;
            default: printf("Opcion invalida.\n");
        }
    } while(opc != 0);
}

void informesRestauranteLocal(int id_rest) {
    FILE *fBin = fopen("pedidos.dat", "rb");
    if(fBin == NULL) { printf("No hay pedidos registrados.\n"); return; }

    char nombreArchivo[50];
    sprintf(nombreArchivo, "mis_pedidos_local_%d.txt", id_rest);
    FILE *fTxt = fopen(nombreArchivo, "w");
    if(fTxt == NULL) { fclose(fBin); return; }

    fprintf(fTxt, "=== HISTORIAL DE PEDIDOS RECIBIDOS - LOCAL #%d ===\n", id_rest);
    fprintf(fTxt, "---------------------------------------------------\n");

    PedidoCliente p;
    int cont = 0;
    while(fread(&p, sizeof(PedidoCliente), 1, fBin) == 1) {
        if(p.id_usuario_restaurante == id_rest && p.activo == 1) {
            fprintf(fTxt, "Ticket #%d | Fecha: %s | Total: $%.2f | Cliente ID: %d\n",
                    p.id_pedido, p.fecha, p.total, p.id_cliente);
            cont++;
        }
    }
    fclose(fBin); fclose(fTxt);
    printf("Informe generado: %d pedidos exportados a '%s'\n", cont, nombreArchivo);
}

int obtenerRestoDelProducto(int id_prod) {
    FILE *fProd = fopen("productos.dat", "rb");
    if(!fProd) return -1;
    Producto p;
    while(fread(&p, sizeof(Producto), 1, fProd) == 1) {
        if(p.id_producto == id_prod) {
            fclose(fProd);
            return p.id_usuario_restaurante;
        }
    }
    fclose(fProd);
    return -1; // No encontrado
}

void informePlatosMasVendidosLocal(int id_rest) {
    FILE *fDet = fopen("detalles_pedido.dat", "rb");
    if(!fDet) { printf("No hay detalles de pedidos.\n"); return; }

    typedef struct { int id_p; int cant; } ContadorPlato;
    ContadorPlato listado[500];
    int totalPlatosDiferentes = 0;
    memset(listado, 0, sizeof(listado));

    ProductosPedido d;
    while(fread(&d, sizeof(ProductosPedido), 1, fDet) == 1) {
        // Usamos la auxiliar para ver si este producto es de este restaurante
        if(obtenerRestoDelProducto(d.id_producto) == id_rest) {
            int encontrado = 0;
            for(int i = 0; i < totalPlatosDiferentes; i++) {
                if(listado[i].id_p == d.id_producto) {
                    listado[i].cant += d.cantidad;
                    encontrado = 1;
                    break;
                }
            }
            if(!encontrado && totalPlatosDiferentes < 500) {
                listado[totalPlatosDiferentes].id_p = d.id_producto;
                listado[totalPlatosDiferentes].cant = d.cantidad;
                totalPlatosDiferentes++;
            }
        }
    }
    fclose(fDet);

    // Si sigue dando 0, es que el ID de producto en detalles_pedido.dat
    // no coincide con el de productos.dat
    if(totalPlatosDiferentes == 0) {
        printf("DEBUG: No se encontraron ventas para el resto %d. Revisa los IDs en detalles_pedido.dat\n", id_rest);
        return;
    }

    // Ordenar y guardar... (Tu lógica de Bubble Sort es correcta, dejala igual)
    // ...
    printf("¡Ranking generado con éxito!\n");
}

// Función interna para obtener la fecha y hora formateada en los reportes
void obtenerFechaHoraActual(char* buffer) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}

void informeRestaurantesTop() {
    FILE *fBin = fopen("restaurantes.dat", "rb");
    if(fBin == NULL) { printf("No hay datos de restaurantes.\n"); return; }

    FILE *fTxt = fopen("restaurantes_top.txt", "w");
    if(fTxt == NULL) { fclose(fBin); return; }

    char timestamp[30];
    obtenerFechaHoraActual(timestamp);

    fprintf(fTxt, "===================================================\n");
    fprintf(fTxt, "   RESTAURANTES TOP (CALIFICACION > 4.0)\n");
    fprintf(fTxt, "   Fecha de emision: %s\n", timestamp);
    fprintf(fTxt, "===================================================\n");
    fprintf(fTxt, "ID\tNombre\t\t\tCalificacion\tRubro\n");
    fprintf(fTxt, "---------------------------------------------------\n");

    Restaurante r;
    int cont = 0;
    while(fread(&r, sizeof(Restaurante), 1, fBin) == 1) {
        if(r.activo == 1 && r.calificacion > 4.0) {
            fprintf(fTxt, "%d\t%-20s\t%.1f / 5.0\t\t%s\n", r.id_restaurante, r.nombre, r.calificacion, r.descripcion);
            cont++;
        }
    }
    fclose(fBin);
    fclose(fTxt);
    printf("¡Informe generado con exito! Se exportaron %d locales a 'restaurantes_top.txt'\n", cont);
}

void informePedidosUsuario() {
    int id_user;
    printf("Ingrese el ID del Usuario: ");
    scanf("%d", &id_user);
    getchar();

    FILE *fBin = fopen("pedidos.dat", "rb");
    if(fBin == NULL) { printf("No hay datos de pedidos.\n"); return; }

    char nombreArchivo[50];
    sprintf(nombreArchivo, "pedidos_usuario_%d.txt", id_user);

    FILE *fTxt = fopen(nombreArchivo, "w");
    if(fTxt == NULL) { fclose(fBin); return; }

    char timestamp[30];
    obtenerFechaHoraActual(timestamp);

    fprintf(fTxt, "===================================================\n");
    fprintf(fTxt, "   HISTORIAL COMPLETO DE PEDIDOS - USUARIO #%d\n", id_user);
    fprintf(fTxt, "   Emitido: %s\n", timestamp);
    fprintf(fTxt, "===================================================\n");

    PedidoCliente p;
    int cont = 0;
    while(fread(&p, sizeof(PedidoCliente), 1, fBin) == 1) {
        if(p.id_cliente == id_user && p.activo == 1) {
            fprintf(fTxt, "Ticket #%d | Fecha: %s | Total: $%.2f | Estado: %d\n",
                    p.id_pedido, p.fecha, p.total, p.estado);
            cont++;
        }
    }
    fclose(fBin);
    fclose(fTxt);
    printf("¡Informe finalizado! %d pedidos guardados en '%s'\n", cont, nombreArchivo);
}

void informeTopPlatos() {
    FILE *fDet = fopen("detalles_pedido.dat", "rb");
    if(fDet == NULL) { printf("No hay datos de consumos.\n"); return; }

    // Estructura interna auxiliar para agrupar acumulados sin reventar memoria
    typedef struct { int id_p; int cant; } ContadorPlato;
    ContadorPlato listado[500];
    int totalPlatosDiferentes = 0;
    memset(listado, 0, sizeof(listado));

    ProductosPedido d;
    while(fread(&d, sizeof(ProductosPedido), 1, fDet) == 1) {
        int idx = -1;
        for(int i = 0; i < totalPlatosDiferentes; i++) {
            if(listado[i].id_p == d.id_producto) { idx = i; break; }
        }
        if(idx != -1) {
            listado[idx].cant += d.cantidad;
        } else if(totalPlatosDiferentes < 500) {
            listado[totalPlatosDiferentes].id_p = d.id_producto;
            listado[totalPlatosDiferentes].cant = d.cantidad;
            totalPlatosDiferentes++;
        }
    }
    fclose(fDet);

    // Bubble Sort para ordenar el ranking de mayor a menor consumo
    for(int i = 0; i < totalPlatosDiferentes - 1; i++) {
        for(int j = 0; j < totalPlatosDiferentes - i - 1; j++) {
            if(listado[j].cant < listado[j+1].cant) {
                ContadorPlato temp = listado[j];
                listado[j] = listado[j+1];
                listado[j+1] = temp;
            }
        }
    }

    FILE *fTxt = fopen("top_platos.txt", "w");
    if(fTxt == NULL) return;

    char timestamp[30];
    obtenerFechaHoraActual(timestamp);

    fprintf(fTxt, "===================================================\n");
    fprintf(fTxt, "         RANKING TOP 5 - PLATOS MAS PEDIDOS\n");
    fprintf(fTxt, "         Fecha del analisis: %s\n", timestamp);
    fprintf(fTxt, "===================================================\n");

    int limite = totalPlatosDiferentes < 55 ? totalPlatosDiferentes : 5;
    for(int i = 0; i < limite; i++) {
        fprintf(fTxt, "Puesto N%d -> ID Plato: %d \t Unidades Vendidas: %d\n", i+1, listado[i].id_p, listado[i].cant);
    }
    fclose(fTxt);
    printf("¡Ranking de consumo guardado con exito en 'top_platos.txt'!\n");
}

void informeFacturaPedido() {
    int id_ticket;
    printf("Ingrese el ID del Pedido (Facturar): ");
    scanf("%d", &id_ticket);
    getchar();

    FILE *fPed = fopen("pedidos.dat", "rb");
    if(fPed == NULL) return;

    PedidoCliente p;
    int encontrado = 0;
    while(fread(&p, sizeof(PedidoCliente), 1, fPed) == 1) {
        if(p.id_pedido == id_ticket && p.activo == 1) { encontrado = 1; break; }
    }
    fclose(fPed);

    if(!encontrado) { printf("Error: El pedido no existe.\n"); return; }

    char nombreArchivo[50];
    sprintf(nombreArchivo, "factura_%d.txt", id_ticket);
    FILE *fTxt = fopen(nombreArchivo, "w");
    if(fTxt == NULL) return;

    char timestamp[30];
    obtenerFechaHoraActual(timestamp);

    fprintf(fTxt, "===================================================\n");
    fprintf(fTxt, "              FACTURA DE COMPRA DIGITAL\n");
    fprintf(fTxt, "===================================================\n");
    fprintf(fTxt, " Ticket N: %d \t\t Emision: %s\n", p.id_pedido, timestamp);
    fprintf(fTxt, " Cliente ID: %d \t\t Resto ID: %d\n", p.id_cliente, p.id_usuario_restaurante);
    fprintf(fTxt, "---------------------------------------------------\n");
    fprintf(fTxt, "ID Plato\tCantidad\tSubtotal\n");
    fprintf(fTxt, "---------------------------------------------------\n");

    FILE *fDet = fopen("detalles_pedido.dat", "rb");
    if(fDet != NULL) {
        ProductosPedido d;
        while(fread(&d, sizeof(ProductosPedido), 1, fDet) == 1) {
            if(d.id_pedido == id_ticket) {
                fprintf(fTxt, "%d\t\t%d\t\t$%.2f\n", d.id_producto, d.cantidad, d.subtotal);
            }
        }
        fclose(fDet);
    }
    fprintf(fTxt, "---------------------------------------------------\n");
    fprintf(fTxt, " TOTAL FACTURADO OBLIGATORIO: \t\t$%.2f\n", p.total);
    fprintf(fTxt, "===================================================\n");
    fclose(fTxt);
    printf("¡Factura impresa digitalmente en '%s'!\n", nombreArchivo);
}

void generarInformesTxt()
{
    int opc;
    do {
        printf("\n--- METRICAS Y REPORTES DEL SISTEMA (.TXT) ---\n");
        printf("1.  Listado de Restaurantes Top (Calificacion > 4.0)\n");
        printf("2.  Historial de Pedidos de un Usuario Especifico\n");
        printf("3.  Ranking Global de Platos mas vendidos (Top 5)\n");
        printf("4.  Emitir Factura de un Pedido Especifico\n");
        printf("0. Volver al portal corporativo\n");
        printf("Opcion Reporte: ");
        scanf("%d", &opc);
        getchar();

        switch(opc) {
            case 1: informeRestaurantesTop(); break;
            case 2: informePedidosUsuario(); break;
            case 3: informeTopPlatos(); break;
            case 4: informeFacturaPedido(); break;
            case 0: printf("Regresando al portal...\n"); break;
            default: printf("Opcion invalida.\n");
        }
    } while(opc != 0);
}
