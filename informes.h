#ifndef INFORMES_H
#define INFORMES_H

#include "estructuras.h"
int obtenerRestoDelProducto(int id_prod);
void informePlatosMasVendidosLocal(int id_rest);
void portalReportesRestaurante(int id_rest);
void informesRestauranteLocal(int id_rest);
// Función contenedora del menú de listados en texto
void generarInformesTxt();

#endif // INFORMES_H
