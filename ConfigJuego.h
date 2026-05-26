#ifndef CONFIG_JUEGO_H
#define CONFIG_JUEGO_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "AVRCompat.h"

// ==========================
// CONFIGURACION GENERAL
// ==========================

const uint8_t DIRECCION_LCD = 0x27;
const uint8_t COLUMNAS_LCD = 20;
const uint8_t FILAS_LCD = 4;

const uint8_t FILA_JUEGO_MIN = 1;
const uint8_t FILA_JUEGO_MAX = 3;
const uint8_t FILAS_JUEGO = 3;

const uint8_t COLUMNA_MIN = 0;
const uint8_t COLUMNA_MAX = COLUMNAS_LCD - 1;

const uint16_t PUNTAJE_MAXIMO = 999;
const uint8_t VIDAS_INICIALES = 3;
const uint8_t NIVEL_INICIAL = 1;
const uint8_t NIVEL_MAXIMO = 8;

const uint16_t PUNTAJE_NIVEL_2 = 8;
const uint16_t PUNTAJE_NIVEL_3 = 20;
const uint16_t PUNTAJE_NIVEL_4 = 40;
const uint16_t PUNTAJE_NIVEL_5 = 70;
const uint16_t PUNTAJE_NIVEL_6 = 110;
const uint16_t PUNTAJE_NIVEL_7 = 160;
const uint16_t PUNTAJE_NIVEL_8 = 220;

const uint8_t TOTAL_BALAS = FILAS_JUEGO;
const uint8_t TOTAL_MARCADORES = 3;
const uint8_t LETRAS_NOMBRE = 3;
const uint8_t COLUMNA_NOMBRE_NUEVO = 13;

const uint8_t TOTAL_OPCIONES_MENU = 3;
const uint8_t ULTIMA_OPCION_MENU = TOTAL_OPCIONES_MENU - 1;
const uint8_t TOTAL_INDICES_BOTON = 8;

const uint8_t TOTAL_TIPOS_ENEMIGO = 3;
const uint8_t DISTANCIA_MINIMA_ENEMIGO = 3;
const uint8_t ALTURA_CARACTER_LCD = 8;
const uint8_t VALOR_CACHE_INVALIDO = 255;
const unsigned long VELOCIDAD_SERIAL = 9600;

const uint8_t PORCENTAJE_DURACION_BASE = 100;
const uint8_t REDUCCION_MUSICA_POR_NIVEL = 3;
const uint16_t DURACION_MINIMA_NOTA_MS = 25;
const uint8_t PORCENTAJE_TONO_MUSICA = 82;

// ==========================
// PINES
// ==========================

#define BOTON_ARRIBA     2
#define BOTON_ABAJO      3
#define BOTON_IZQUIERDA  4
#define BOTON_DERECHA    5
#define BOTON_DISPARO_1  6
#define BOTON_DISPARO_2  7

#define BUZZER_MUSICA    9
#define BUZZER_EFECTO    11

// ==========================
// CARACTERES LCD
// ==========================

#define CAR_JUGADOR    0
#define CAR_ENEMIGO_1  1
#define CAR_BALA       2
#define CAR_ESPECIAL   3
#define CAR_EXPLOSION  4
#define CAR_ENEMIGO_2  5
#define CAR_ENEMIGO_3  6
#define CAR_SELECTOR   7

#define MAXIMO_ENEMIGOS 12
#define MAXIMO_ENEMIGOS_PATRON 3
#define TAMANIO_BUFFER_COMANDO 41

#endif
