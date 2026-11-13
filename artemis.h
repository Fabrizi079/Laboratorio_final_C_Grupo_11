#ifndef ARTEMIS_H
#define ARTEMIS_H

/* ---------- Constantes fisicas  ---------- */
#define G_CONST        6.674e-11      /* N*m^2/kg^2 */
#define MASA_TIERRA    5.972e24       /* kg */
#define RADIO_TIERRA   6371000.0      /* m */
#define PI             3.14159265358979323846

/* ---------- Datos base de la consigna ---------- */
#define ALTITUD_BASE   192000.0       /* m */
#define MASA_BASE      33000.0        /* kg */
#define ACEL_PULSO_BASE 0.50          /* m/s^2 */
#define DUR_PULSO_BASE  10.0          /* s */

/* ---------- Limites de validacion ---------- */
#define ALT_MIN        160000.0       /* por debajo la atmosfera frena la nave */
#define ALT_MAX        2000000.0
#define MASA_MIN       1000.0
#define MASA_MAX       100000.0
#define ACEL_MAX       5.0
#define DUR_MAX        60.0

/* ---------- Parametros del modo mision ---------- */
#define MAX_TURNOS     6
#define OBJETIVO_ESTABLES 3
#define TOL_ESTABLE    1.0            /* % de error aceptado como orbita estable */
#define TOL_ELIPTICA   5.0            /* % hasta orbita eliptica aceptable */
#define RUIDO_MAX      0.3            /* % de perturbacion aleatoria */

typedef struct {
    double altitud;       /* m */
    double radio;         /* m (RT + h) */
    double masa_nave;     /* kg */
    double f_grav;        /* N */
    double v_orbital;     /* m/s */
    double periodo;       /* s */
    double a_centripeta;  /* m/s^2 */
} Orbita;

typedef struct {
    double aceleracion;   /* m/s^2 */
    double duracion;      /* s */
    double delta_v;       /* m/s, componente lateral */
    double desviacion;    /* m, desplazamiento lateral */
} PulsoLateral;

typedef enum {
    RES_ESTABLE,
    RES_ELIPTICA,
    RES_DECAE,
    RES_EXCENTRICA,
    RES_ESCAPE
} Resultado;

typedef struct {
    int       numero;
    Orbita    orbita;        /* orbita objetivo del turno */
    double    v_ingresada;   /* m/s, lo que ingreso el jugador */
    double    ruido_pct;     /* perturbacion aplicada, % */
    double    v_efectiva;    /* m/s, con perturbacion */
    double    error_pct;     /* % respecto de v_orbital */
    Resultado resultado;
    int       puntos;
} Turno;

typedef struct {
    Turno turnos[MAX_TURNOS];
    int   cantidad;          /* turnos jugados */
    int   estables;
    int   puntaje;
} Mision;

/* ---------- Calculos fisicos ---------- */
void inicializarOrbita(Orbita *o, double altitud, double masa);
void calcularOrbita(Orbita *o);
void calcularPulso(PulsoLateral *p, double aceleracion, double duracion);

/* ---------- Reportes de analisis ---------- */
void mostrarReporteOrbita(const Orbita *o);
void mostrarDemostracion(const Orbita *o);
void mostrarPulso(const PulsoLateral *p, const Orbita *o);

/* ---------- Modo mision (juego por turnos) ---------- */
void      inicializarMision(Mision *m);
int       misionTerminada(const Mision *m);
void      prepararTurno(Mision *m);                 /* sortea altitud y masa */
void      resolverTurno(Mision *m, double v_ingresada);
Resultado clasificarVelocidad(double v_efectiva, double v_orbital, double *error_pct);
const char *textoResultado(Resultado r);
void      mostrarTurno(const Turno *t);
void      mostrarReporteFinal(const Mision *m);

#endif
