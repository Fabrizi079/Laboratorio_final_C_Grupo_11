#ifndef VALIDACIONES_H
#define VALIDACIONES_H

/* Lectura segura: nunca usan scanf directo, asi que no se rompen ni
   entran en bucle infinito ante letras u otras entradas erroneas. */
int    leerEntero(const char *mensaje, int min, int max);
double leerDouble(const char *mensaje, double min, double max);
char   leerSiNo(const char *mensaje);          /* devuelve 's' o 'n' */

/* Mensajes de interfaz */
void mostrarError(const char *texto);
void mostrarAviso(const char *texto);
void pausar(void);

#endif
