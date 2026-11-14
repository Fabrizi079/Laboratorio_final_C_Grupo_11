#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "artemis.h"
#include "validaciones.h"

static void mostrarMenu(const Orbita *o) {
    printf("\n==============================================\n");
    printf("   ARTEMIS - ORION EN ORBITA TERRESTRE BAJA\n");
    printf("==============================================\n");
    printf(" Datos actuales: h = %.1f km, m = %.1f kg\n", o->altitud / 1000.0, o->masa_nave);
    printf(" 1) Analisis orbital (v, T, a centripeta, fuerza)\n");
    printf(" 2) Demostracion: gravedad = fuerza centripeta\n");
    printf(" 3) Pulso lateral\n");
    printf(" 4) Cambiar altitud y masa\n");
    printf(" 5) MODO MISION (juego por turnos)\n");
    printf(" 6) Reporte final de la ultima mision\n");
    printf(" 0) Salir\n");
}

static void cambiarDatos(Orbita *o) {
    double h_km, masa;
    h_km = leerDouble("Altitud en km (160 a 2000): ", ALT_MIN / 1000.0, ALT_MAX / 1000.0);
    masa = leerDouble("Masa de la nave en kg (1000 a 100000): ", MASA_MIN, MASA_MAX);
    inicializarOrbita(o, h_km * 1000.0, masa);
    printf("Datos actualizados.\n");
}

static void analizarPulso(const Orbita *o) {
    PulsoLateral p;
    double a, dt;

    if (leerSiNo("Usar los datos de la consigna (0.50 m/s^2, 10 s)? (s/n): ") == 's') {
        a  = ACEL_PULSO_BASE;
        dt = DUR_PULSO_BASE;
    } else {
        a  = leerDouble("Aceleracion del pulso en m/s^2 (0 a 5): ", 0.0, ACEL_MAX);
        dt = leerDouble("Duracion del pulso en s (0 a 60): ", 0.0, DUR_MAX);
    }
    calcularPulso(&p, a, dt);
    mostrarPulso(&p, o);
}

static void jugarMision(Mision *m) {
    double v;

    inicializarMision(m);
    printf("\n=== MODO MISION ===\n");
    printf("Debe insertar a Orion en orbita circular estable. En cada turno se\n");
    printf("sortea una altitud: calcule la velocidad orbital necesaria\n");
    printf("(v = sqrt(G*MT/r)) e ingresela. Hay una pequena perturbacion aleatoria.\n");
    printf("Objetivo: %d orbitas estables (error <= %.1f %%) en %d turnos.\n",
           OBJETIVO_ESTABLES, TOL_ESTABLE, MAX_TURNOS);

    while (!misionTerminada(m)) {
        Turno *t;
        prepararTurno(m);
        t = &m->turnos[m->cantidad];

        printf("\n--- TURNO %d de %d | estables: %d/%d | puntaje: %d ---\n",
               t->numero, MAX_TURNOS, m->estables, OBJETIVO_ESTABLES, m->puntaje);
        printf("Constante de gravitacion : 6,6743 x 10^-11 m^3/(kg x s^2)\n");
        printf("Masa de la Tierra        : 5,972 x 10^24 kg\n");
        printf("Radio orbital            : %.1f m\n", t->orbita.radio);

        v = leerDouble("Velocidad de inyeccion en m/s (1000 a 15000): ", 1000.0, 15000.0);
        resolverTurno(m, v);
        mostrarTurno(&m->turnos[m->cantidad - 1]);
    }
    mostrarReporteFinal(m);
}

int main(void) {
    Orbita orbita;
    Mision mision;
    int opcion;

    srand((unsigned)time(NULL));
    inicializarOrbita(&orbita, ALTITUD_BASE, MASA_BASE);
    inicializarMision(&mision);

    do {
        mostrarMenu(&orbita);
        opcion = leerEntero("Opcion: ", 0, 6);
        switch (opcion) {
            case 1: mostrarReporteOrbita(&orbita); pausar(); break;
            case 2: mostrarDemostracion(&orbita);  pausar(); break;
            case 3: analizarPulso(&orbita);        pausar(); break;
            case 4: cambiarDatos(&orbita);         break;
            case 5: jugarMision(&mision);          pausar(); break;
            case 6: mostrarReporteFinal(&mision);  pausar(); break;
            case 0: printf("Fin del programa. Buen viaje, Orion.\n"); break;
        }
    } while (opcion != 0);

    return 0;
}
