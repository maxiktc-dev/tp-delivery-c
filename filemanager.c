#include "filemanager.h"

FILE* abrirArchivo(const char* nombreArchivo, const char* modo)
{
    FILE* f = fopen(nombreArchivo, modo);

    if(f == NULL)
    {
        printf("Error critico: No se pudo acceder al archivo %s\n", nombreArchivo);
    }

    return f;
}

int escribirRegistro(FILE* f, const void* registro, size_t tamano)
{
    // Escribimos 1 registro del tamaño especificado
    if(fwrite(registro, tamano, 1, f) == 1)
    {
        return 1; // Éxito
    }
    return 0; // Error al escribir
}

int leerRegistro(FILE* f, void* buffer, size_t tamano)
{
    // Leemos 1 registro y lo metemos en el buffer
    if(fread(buffer, tamano, 1, f) == 1)
    {
        return 1; // Leyo bien
    }
    return 0; // EOF o Error
}
