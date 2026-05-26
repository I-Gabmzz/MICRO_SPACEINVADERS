#include "Puntaje.h"
#include "Marcadores.h"
#include "Pantallas.h"
#include "Sonido.h"

void aumentarPuntaje(uint8_t puntos) {
  puntaje += puntos;

  if (puntaje > PUNTAJE_MAXIMO) {
    puntaje = PUNTAJE_MAXIMO;
  }

  actualizarDificultad();
  actualizarBarraEstado(true);
}

void actualizarDificultad() {
  if (puntaje >= PUNTAJE_NIVEL_8) {
    nivel = NIVEL_MAXIMO;
    velocidadEnemigoMs = 205;
    intervaloOleadaMs = 760;
    maximoEnemigosActivos = 12;
    aparicionXMin = 12;
    aparicionXMax = COLUMNA_MAX;
    indicePatronMaximo = 7;
  }
  else if (puntaje >= PUNTAJE_NIVEL_7) {
    nivel = 7;
    velocidadEnemigoMs = 240;
    intervaloOleadaMs = 900;
    maximoEnemigosActivos = 10;
    aparicionXMin = 13;
    aparicionXMax = COLUMNA_MAX;
    indicePatronMaximo = 7;
  }
  else if (puntaje >= PUNTAJE_NIVEL_6) {
    nivel = 6;
    velocidadEnemigoMs = 285;
    intervaloOleadaMs = 1050;
    maximoEnemigosActivos = 9;
    aparicionXMin = 14;
    aparicionXMax = COLUMNA_MAX;
    indicePatronMaximo = 7;
  }
  else if (puntaje >= PUNTAJE_NIVEL_5) {
    nivel = 5;
    velocidadEnemigoMs = 335;
    intervaloOleadaMs = 1220;
    maximoEnemigosActivos = 8;
    aparicionXMin = 15;
    aparicionXMax = COLUMNA_MAX;
    indicePatronMaximo = 7;
  }
  else if (puntaje >= PUNTAJE_NIVEL_4) {
    nivel = 4;
    velocidadEnemigoMs = 400;
    intervaloOleadaMs = 1450;
    maximoEnemigosActivos = 7;
    aparicionXMin = 16;
    aparicionXMax = COLUMNA_MAX;
    indicePatronMaximo = 7;
  }
  else if (puntaje >= PUNTAJE_NIVEL_3) {
    nivel = 3;
    velocidadEnemigoMs = 480;
    intervaloOleadaMs = 1700;
    maximoEnemigosActivos = 6;
    aparicionXMin = 17;
    aparicionXMax = COLUMNA_MAX;
    indicePatronMaximo = 6;
  }
  else if (puntaje >= PUNTAJE_NIVEL_2) {
    nivel = 2;
    velocidadEnemigoMs = 560;
    intervaloOleadaMs = 1950;
    maximoEnemigosActivos = 5;
    aparicionXMin = 18;
    aparicionXMax = COLUMNA_MAX;
    indicePatronMaximo = 5;
  }
  else {
    nivel = NIVEL_INICIAL;
    velocidadEnemigoMs = 650;
    intervaloOleadaMs = 2200;
    maximoEnemigosActivos = 4;
    aparicionXMin = COLUMNA_MAX;
    aparicionXMax = COLUMNA_MAX;
    indicePatronMaximo = 3;
  }
}

void perderVida() {
  if (modoAdmin) {
    return;
  }

  if (millis() - ultimoDanioMs < tiempoInvulnerableDanioMs) {
    return;
  }

  ultimoDanioMs = millis();

  if (vidas > 0) {
    vidas--;
  }

  efectoDanio();
  actualizarBarraEstado(true);

  if (vidas <= 0) {
    finalizarPartida();
  }
}

void finalizarPartida() {
  juegoPausado = false;
  areaJuegoPendiente = false;

  for (uint8_t i = 0; i < MAXIMO_ENEMIGOS; i++) {
    enemigoActivo[i] = false;
  }

  for (uint8_t fila = 0; fila < TOTAL_BALAS; fila++) {
    balaActiva[fila] = false;
  }

  proyectilEspecialActivo = false;
  explosionActiva = false;

  efectoFinPartida();

  posicionNuevoMarcador = obtenerPosicionMarcador(puntaje);

  if (posicionNuevoMarcador != -1) {
    nombreNuevo[0] = 'A';
    nombreNuevo[1] = 'A';
    nombreNuevo[2] = 'A';
    nombreNuevo[LETRAS_NOMBRE] = '\0';
    indiceNombre = 0;

    mostrarNuevaMarca();
  }
  else {
    mostrarFinPartida();
  }
}
