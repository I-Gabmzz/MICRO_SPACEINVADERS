#ifndef ESTADO_JUEGO_H
#define ESTADO_JUEGO_H

#include "ConfigJuego.h"

struct Marcador {
  char nombre[LETRAS_NOMBRE + 1];
  uint16_t puntos;
};

enum Pantalla {
  PANTALLA_INICIO,
  PANTALLA_MENU,
  PANTALLA_MARCADORES,
  PANTALLA_JUEGO,
  PANTALLA_NUEVA_MARCA,
  PANTALLA_FIN_PARTIDA
};

extern LiquidCrystal_I2C lcd;
extern Pantalla pantallaActual;
extern uint8_t opcionMenu;
extern uint8_t jugadorX;
extern uint8_t jugadorY;
extern uint16_t puntaje;
extern uint8_t vidas;
extern uint8_t nivel;
extern unsigned long ultimoDanioMs;
extern const unsigned long tiempoInvulnerableDanioMs;
extern bool modoAdmin;
extern bool juegoPausado;
extern char comandoSerial[TAMANIO_BUFFER_COMANDO];
extern uint8_t indiceComando;
extern Marcador marcadores[TOTAL_MARCADORES];
extern const int EEPROM_DIRECCION_FIRMA;
extern const int EEPROM_DIRECCION_DATOS;
extern const byte EEPROM_FIRMA;
extern char nombreNuevo[LETRAS_NOMBRE + 1];
extern uint8_t indiceNombre;
extern int8_t posicionNuevoMarcador;
extern bool balaActiva[TOTAL_BALAS];
extern int8_t balaX[TOTAL_BALAS];
extern uint8_t balaY[TOTAL_BALAS];
extern unsigned long ultimoMovimientoBalaMs[TOTAL_BALAS];
extern const unsigned long velocidadBalaMs;
extern bool enemigoActivo[MAXIMO_ENEMIGOS];
extern int8_t enemigoX[MAXIMO_ENEMIGOS];
extern uint8_t enemigoY[MAXIMO_ENEMIGOS];
extern uint8_t enemigoTipo[MAXIMO_ENEMIGOS];
extern unsigned long ultimoMovimientoEnemigoMs;
extern unsigned long ultimaOleadaMs;
extern unsigned long velocidadEnemigoMs;
extern unsigned long intervaloOleadaMs;
extern uint8_t maximoEnemigosActivos;
extern int8_t aparicionXMin;
extern int8_t aparicionXMax;
extern uint8_t indicePatronMaximo;
extern bool proyectilEspecialActivo;
extern int8_t especialX;
extern uint8_t especialY;
extern unsigned long ultimoMovimientoEspecialMs;
extern const unsigned long velocidadEspecialMs;
extern bool explosionActiva;
extern int8_t explosionX;
extern uint8_t explosionY;
extern bool mascaraExplosion[FILAS_JUEGO][FILAS_JUEGO];
extern unsigned long inicioExplosionMs;
extern const unsigned long duracionExplosionMs;
extern unsigned long ultimoEspecialMs;
extern const unsigned long recargaEspecialMs;
extern bool especialUsadoUnaVez;
extern bool ultimoArriba;
extern bool ultimoAbajo;
extern bool ultimoDerecha;
extern bool ultimoIzquierda;
extern bool ultimoDisparo1;
extern bool ultimoDisparo2;
extern unsigned long ultimoCambioBotonMs[TOTAL_INDICES_BOTON];
extern const unsigned long tiempoReboteMs;
extern unsigned long ultimaActualizacionBarraMs;
extern bool areaJuegoPendiente;
extern uint8_t cacheJuego[FILAS_JUEGO][COLUMNAS_LCD];
extern bool musicaActiva;
extern uint8_t indiceMusica;
extern bool notaMusicaIniciada;
extern unsigned long inicioNotaMusicaMs;
extern const uint16_t *frecuenciasEfecto;
extern const uint16_t *duracionesEfecto;
extern uint8_t longitudEfecto;
extern uint8_t indiceEfecto;
extern bool efectoActivo;
extern unsigned long inicioNotaEfectoMs;

#endif
