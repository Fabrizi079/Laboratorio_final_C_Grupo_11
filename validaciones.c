#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include "validaciones.h"

#define TAM_BUF 64

void mostrarError(const char *texto) { printf("  [ERROR] %s\n", texto); }
void mostrarAviso(const char *texto) { printf("  [AVISO] %s\n", texto); }

/* Lee una linea completa. Devuelve 0 si hubo EOF. Descarta el resto si la
   linea era mas larga que el buffer. */
static int leerLinea(char *buf, int tam) {
    if (fgets(buf, tam, stdin) == NULL)
        return 0;
    if (strchr(buf, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
    return 1;
}

static int soloEspacios(const char *s) {
    while (*s) {
        if (!isspace((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

int leerEntero(const char *mensaje, int min, int max) {
    char buf[TAM_BUF], *fin;
    long v;
    for (;;) {
        printf("%s", mensaje);
        if (!leerLinea(buf, TAM_BUF)) {
            mostrarAviso("Fin de entrada. Saliendo.");
            exit(0);
        }
        errno = 0;
        v = strtol(buf, &fin, 10);
        if (fin == buf || !soloEspacios(fin) || errno == ERANGE) {
            mostrarError("Entrada invalida: ingrese un numero entero.");
            continue;
        }
        if (v < min || v > max) {
            printf("  [ERROR] Fuera de rango: debe estar entre %d y %d.\n", min, max);
            continue;
        }
        return (int)v;
    }
}

double leerDouble(const char *mensaje, double min, double max) {
    char buf[TAM_BUF], *fin, *coma;
    double v;
    for (;;) {
        printf("%s", mensaje);
        if (!leerLinea(buf, TAM_BUF)) {
            mostrarAviso("Fin de entrada. Saliendo.");
            exit(0);
        }
        /* acepta coma decimal (7793,5) ademas de punto */
        coma = strchr(buf, ',');
        if (coma) *coma = '.';
        errno = 0;
        v = strtod(buf, &fin);
        if (fin == buf || !soloEspacios(fin) || errno == ERANGE) {
            mostrarError("Entrada invalida: ingrese un numero (ej: 7793.5).");
            continue;
        }
        if (v != v || v < min || v > max) {   /* v != v detecta NaN */
            printf("  [ERROR] Fuera de rango: debe estar entre %.2f y %.2f.\n", min, max);
            continue;
        }
        return v;
    }
}

char leerSiNo(const char *mensaje) {
    char buf[TAM_BUF], c;
    for (;;) {
        printf("%s", mensaje);
        if (!leerLinea(buf, TAM_BUF)) {
            mostrarAviso("Fin de entrada. Saliendo.");
            exit(0);
        }
        c = (char)tolower((unsigned char)buf[0]);
        if ((c == 's' || c == 'n') && soloEspacios(buf + 1))
            return c;
        mostrarError("Responda 's' (si) o 'n' (no).");
    }
}

void pausar(void) {
    char buf[TAM_BUF];
    printf("\nPresione ENTER para continuar...");
    leerLinea(buf, TAM_BUF);
}
