#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdio.h>
#include <stdlib.h>

// Abre un archivo y avisa si hay error
FILE* abrirArchivo(const char* nombreArchivo, const char* modo);

// Escribe cualquier tipo de struct genérico en el disco
int escribirRegistro(FILE* f, const void* registro, size_t tamano);

// Lee cualquier tipo de struct genérico desde el disco
int leerRegistro(FILE* f, void* buffer, size_t tamano);

#endif // FILEMANAGER_H
