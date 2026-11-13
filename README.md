# Lab_Final_C_Grupo11

# Trabajo Final Integrador - Laboratorio de C

## Integrantes

* Cuenca Danna
* Duarte Santiago
* Fernandez Eugenia
* Gutierrez Fabrizio
* Suarez Mauro

## Comisión

ISI - Comisión C

## Descripción del Proyecto

Programa "ARTEMIS - ORION EN ORBITA TERRESTRE BAJA"

El objetivo de este programa es responder una serie de preguntas relacionadas con la Inserción y Estabilidad en Órbita Terrestre Baja de la nave Orión perteneciente a la misión ARTEMIS, además de incluir un juego didáctico en el cual el usuario simula el lanzamiento de naves a la orbita terrestre, ingresando valores de velocidad los cuales deben encontrarse dentro del rango aceptable en relación también con los otros valores que el juego brinda, y sumando puntos cuando los valores de velocidad se encuentren dentro de este rango

Principales funcionalidades:

* Demostración analítica de que la fuerza de atracción gravitatoria actúa como la fuerza centrípeta del sistema
* Calculo del módulo de la velocidad orbital v
* Calculo de los segundos y minutos le toma a la nave Orion completar una vuelta completa alrededor de la Tierra
* Calculo de la aceleración centrípeta que experimentan los astronautas dentro de la cápsula a partir de la altitud dada por el problema
* Descripcion de la trayectoria resultante tras encender un propulsor auxiliar
* Calculo de la desviación lateral instantánea
* Enseñanza de estos conceptos mediante el uso de un enfoque mas lúdico basado en el juego


## Tecnologías Utilizadas

* C versión C17
* GitHub
* Consola de comandos
* VsCode 

## Funcionamiento y arquitectura
| Archivo | Responsabilidad |
|---|---|
| main.c | Menu principal y flujo general |
| artemis.h | Constantes, structs (Orbita, PulsoLateral, Turno, Mision)|
| artemis.c | Calculos fisicos, logica de turnos, reportes |
| validaciones.h/.c | Lectura de los valores y envió de mensajes de error |

## Escenario y reglas del juego
La nave Orion (33.000 kg) orbita la Tierra en un MCU a h = 192 km (r = RT + h = 6.563 km).
El programa calcula, para los datos ingresados por el usuario:
- Velocidad orbital `v = sqrt(G*MT/r)` y demostracion de que la gravedad es la fuerza centripeta.
- Periodo orbital `T = 2*pi*r/v`.
- Aceleracion centripeta `a = v^2/r`.
- Pulso lateral: `dv = a*dt`, desviacion `d = 1/2*a*dt^2`, y descripcion de la trayectoria.

**Modo Misión (juego por turnos):** en cada turno se brinda una altitud al azar para la cual el jugador debe calcular e ingresar la velocidad de inyección. Hay una perturbación aleatoria de hasta +-0,3 %.
- Error <= 1 %: orbita circular estable (100 pts).
- Error <= 5 %: orbita eliptica aceptable (40 pts).
- Menor a -5 %: la orbita decae. Mayor a +5 %: orbita muy excentrica. >= sqrt(2)*v: escape.
- Se gana con 3 orbitas estables dentro de 6 turnos.

## Instrucciones de Ejecución
Se ingresa en la consola de comandos:

```bash
make          
./artemis    
make clean
```

Para una compilación manual se debe ingresar todo el siguiente comando: 

`gcc -Wall -Wextra -std=c99 -o artemis main.c artemis.c validaciones.c -lm`

## Distribución de Tareas

| Integrante   | Responsabilidad             |
| ------------ | --------------------------- |
| Cuenca Danna | Encargada de la investigación del complemento teórico |
| Duarte Santiago | Encargado de la revisión de errores y la edición de video |
| Fernandez Eugenia | Encargada del control de la interfaz con el usuario |
| Gutierrez Fabrizio | Encargado del cuerpo del Main y de la implementación de los datos fisicos |
| Suarez Mauro | Encargado del modo de Juego y de las validaciones |

## Uso de Inteligencia Artificial

Herramienta utilizada:

* Claude
* Gemini

Casos en donde fue utilizada:

// Agregar despues

## Posibles Mejoras Futuras

* añadir una interfaz grafica que complemente el juego
