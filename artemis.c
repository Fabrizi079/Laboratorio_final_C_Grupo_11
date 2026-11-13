#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "artemis.h"

/* =========================================================
   CALCULOS FISICOS
   ========================================================= */

void inicializarOrbita(Orbita *o, double altitud, double masa) {
    o->altitud   = altitud;
    o->masa_nave = masa;
    o->radio     = RADIO_TIERRA + altitud;
    calcularOrbita(o);
}

void calcularOrbita(Orbita *o) {
    o->f_grav       = G_CONST * MASA_TIERRA * o->masa_nave / (o->radio * o->radio);
    o->v_orbital    = sqrt(G_CONST * MASA_TIERRA / o->radio);
    o->periodo      = 2.0 * PI * o->radio / o->v_orbital;
    o->a_centripeta = (o->v_orbital * o->v_orbital) / o->radio;
}

void calcularPulso(PulsoLateral *p, double aceleracion, double duracion) {
    p->aceleracion = aceleracion;
    p->duracion    = duracion;
    p->delta_v     = aceleracion * duracion;
    p->desviacion  = 0.5 * aceleracion * duracion * duracion;
}

/* =========================================================
   REPORTES DE ANALISIS
   ========================================================= */

void mostrarReporteOrbita(const Orbita *o) {
    printf("\n=== ANALISIS ORBITAL ===\n");
    printf("Altitud h            : %.1f km\n", o->altitud / 1000.0);
    printf("Radio orbital r      : %.1f km\n", o->radio / 1000.0);
    printf("Masa de la nave      : %.1f kg\n", o->masa_nave);
    printf("--------------------------------------------\n");
    printf("Fuerza gravitatoria  : %.4e N\n", o->f_grav);
    printf("Velocidad orbital v  : %.2f m/s (%.3f km/s)\n",
           o->v_orbital, o->v_orbital / 1000.0);
    printf("Periodo T            : %.2f s = %.2f min\n",
           o->periodo, o->periodo / 60.0);
    printf("Aceleracion centrip. : %.4f m/s^2 (%.3f g)\n",
           o->a_centripeta, o->a_centripeta / 9.80);
}

void mostrarDemostracion(const Orbita *o) {
    double fcentripeta = o->masa_nave * o->a_centripeta;
    double dif = fabs(o->f_grav - fcentripeta);

    printf("\n=== DEMOSTRACION: GRAVEDAD = FUERZA CENTRIPETA ===\n");
    printf("1) Sobre la nave solo actua la atraccion gravitatoria, dirigida al\n");
    printf("   centro de la Tierra. En un MCU la fuerza neta debe ser centripeta:\n");
    printf("        G*MT*m / r^2  =  m * v^2 / r\n");
    printf("2) La masa m se cancela (la orbita no depende de la masa de la nave):\n");
    printf("        v^2 = G*MT / r   ->   v = sqrt(G*MT / r)\n");
    printf("3) Verificacion numerica con los datos ingresados:\n");
    printf("        F gravitatoria  = %.6e N\n", o->f_grav);
    printf("        m * v^2 / r     = %.6e N\n", fcentripeta);
    printf("        diferencia      = %.3e N  (error de redondeo)\n", dif);
    printf("4) Resultado: v = %.2f m/s\n", o->v_orbital);
}

void mostrarPulso(const PulsoLateral *p, const Orbita *o) {
    double v_nueva = sqrt(o->v_orbital * o->v_orbital + p->delta_v * p->delta_v);
    double ang = atan(p->delta_v / o->v_orbital) * 180.0 / PI;

    printf("\n=== PULSO LATERAL ===\n");
    printf("Aceleracion a        : %.3f m/s^2 (perpendicular a la velocidad)\n", p->aceleracion);
    printf("Duracion             : %.1f s\n", p->duracion);
    printf("Cambio de velocidad  : %.3f m/s (componente lateral)\n", p->delta_v);
    printf("Desviacion lateral   : %.2f m (d = 1/2 * a * t^2)\n", p->desviacion);
    printf("Rapidez resultante   : %.4f (antes %.4f m/s)\n", v_nueva, o->v_orbital);
    printf("Giro del vector vel. : %.6f grados\n", ang);
    printf("\nTrayectoria: la velocidad deja de ser perpendicular al radio, por lo\n");
    printf("que la orbita circular se transforma en una orbita ligeramente\n");
    printf("distincta (elipse levemente excentrica o plano orbital apenas\n");
    printf("inclinado, segun hacia donde apunte el pulso). La rapidez casi no\n");
    printf("cambia, pero la direccion si: ya no se cumple v = sqrt(G*MT/r) con\n");
    printf("velocidad estrictamente tangencial.\n");
}

    /* =========================================================
   MODO MISION
   ========================================================= */

void inicializarMision(Mision *m) {
    m->cantidad = 0;
    m->estables = 0;
    m->puntaje  = 0;
}

int misionTerminada(const Mision *m) {
    return m->cantidad >= MAX_TURNOS || m->estables >= OBJETIVO_ESTABLES;
}

/* Sortea una altitud (180..1000 km, multiplos de 10) y una masa para el turno */
void prepararTurno(Mision *m) {
    Turno *t = &m->turnos[m->cantidad];
    double alt  = (180 + 10 * (rand() % 83)) * 1000.0;   /* 180..1000 km */
    double masa = 20000.0 + 1000.0 * (rand() % 31);      /* 20000..50000 kg */

    t->numero = m->cantidad + 1;
    inicializarOrbita(&t->orbita, alt, masa);
}

Resultado clasificarVelocidad(double v_efectiva, double v_orbital, double *error_pct) {
    double err = (v_efectiva - v_orbital) / v_orbital * 100.0;
    *error_pct = err;

    if (v_efectiva >= sqrt(2.0) * v_orbital) return RES_ESCAPE;
    if (fabs(err) <= TOL_ESTABLE)            return RES_ESTABLE;
    if (fabs(err) <= TOL_ELIPTICA)           return RES_ELIPTICA;
    if (err < 0)                             return RES_DECAE;
    return RES_EXCENTRICA;
}

const char *textoResultado(Resultado r) {
    switch (r) {
        case RES_ESTABLE:   return "ORBITA CIRCULAR ESTABLE";
        case RES_ELIPTICA:  return "Orbita eliptica aceptable";
        case RES_DECAE:     return "Orbita decae: la nave pierde altura";
        case RES_EXCENTRICA:return "Orbita muy excentrica: riesgo de perder control";
        case RES_ESCAPE:    return "Velocidad de escape: la nave abandona la Tierra";
    }
    return "?";
}

static int puntosPorResultado(Resultado r) {
    switch (r) {
        case RES_ESTABLE:  return 100;
        case RES_ELIPTICA: return 40;
        default:           return 0;
    }
}

void resolverTurno(Mision *m, double v_ingresada) {
    Turno *t = &m->turnos[m->cantidad];

    /* perturbacion aleatoria entre -RUIDO_MAX % y +RUIDO_MAX % */
    t->ruido_pct   = ((rand() / (double)RAND_MAX) * 2.0 - 1.0) * RUIDO_MAX;
    t->v_ingresada = v_ingresada;
    t->v_efectiva  = v_ingresada * (1.0 + t->ruido_pct / 100.0);
    t->resultado   = clasificarVelocidad(t->v_efectiva, t->orbita.v_orbital, &t->error_pct);
    t->puntos      = puntosPorResultado(t->resultado);

    m->puntaje += t->puntos;
    if (t->resultado == RES_ESTABLE) m->estables++;
    m->cantidad++;
}

void mostrarTurno(const Turno *t) {
    printf("\n--- Resultado del turno %d ---\n", t->numero);
    printf("Velocidad ingresada  : %.2f m/s\n", t->v_ingresada);
    printf("Perturbacion (viento solar): %+.3f %%\n", t->ruido_pct);
    printf("Velocidad efectiva   : %.2f m/s\n", t->v_efectiva);
    printf("Velocidad necesaria  : %.2f m/s\n", t->orbita.v_orbital);
    printf("Error                : %+.2f %%\n", t->error_pct);
    printf(">>> %s  (+%d pts)\n", textoResultado(t->resultado), t->puntos);
}

void mostrarReporteFinal(const Mision *m) {
    int i;
    double suma = 0.0;

    printf("\n=========== REPORTE FINAL DE MISION ===========\n");
    if (m->cantidad == 0) {
        printf("Todavia no se jugo ningun turno.\n");
        return;
    }
    printf("Turno | Altitud(km) | v necesaria | v efectiva |  Error%%  | Resultado\n");
    printf("------+-------------+-------------+------------+----------+----------\n");
    for (i = 0; i < m->cantidad; i++) {
        const Turno *t = &m->turnos[i];
        printf("%5d | %11.0f | %11.2f | %10.2f | %+8.2f | %s\n",
               t->numero, t->orbita.altitud / 1000.0, t->orbita.v_orbital,
               t->v_efectiva, t->error_pct, textoResultado(t->resultado));
        suma += fabs(t->error_pct);
    }
    printf("------------------------------------------------\n");
    printf("Turnos jugados        : %d de %d\n", m->cantidad, MAX_TURNOS);
    printf("Orbitas estables      : %d (objetivo: %d)\n", m->estables, OBJETIVO_ESTABLES);
    printf("Error absoluto medio  : %.2f %%\n", suma / m->cantidad);
    printf("Puntaje total         : %d\n", m->puntaje);
    if (m->estables >= OBJETIVO_ESTABLES)
        printf("ESTADO: MISION CUMPLIDA. Orion quedo estable en LEO.\n");
    else if (m->cantidad >= MAX_TURNOS)
        printf("ESTADO: MISION FALLIDA. Se agotaron los turnos.\n");
    else
        printf("ESTADO: Mision en curso.\n");
}
