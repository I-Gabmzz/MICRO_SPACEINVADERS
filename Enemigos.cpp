#include "Enemigos.h"
#include "Colisiones.h"
#include "Pantallas.h"
#include "Puntaje.h"

void inicializarEnemigos() {
  for (uint8_t i = 0; i < MAXIMO_ENEMIGOS; i++) {
    enemigoActivo[i] = false;
    enemigoX[i] = 0;
    enemigoY[i] = FILA_JUEGO_MIN;
    enemigoTipo[i] = 0;
  }
}

uint8_t contarEnemigosActivos() {
  uint8_t total = 0;

  for (uint8_t i = 0; i < MAXIMO_ENEMIGOS; i++) {
    if (enemigoActivo[i]) {
      total++;
    }
  }

  return total;
}

bool posicionLibre(int8_t x, uint8_t y) {
  if (x < COLUMNA_MIN || x > COLUMNA_MAX || y < FILA_JUEGO_MIN || y > FILA_JUEGO_MAX) {
    return false;
  }

  for (uint8_t i = 0; i < MAXIMO_ENEMIGOS; i++) {
    if (enemigoActivo[i]) {
      if (enemigoY[i] == y && abs((int)enemigoX[i] - (int)x) < DISTANCIA_MINIMA_ENEMIGO) {
        return false;
      }
    }
  }

  return true;
}

bool crearEnemigo(int8_t x, uint8_t y) {
  for (uint8_t i = 0; i < MAXIMO_ENEMIGOS; i++) {
    if (!enemigoActivo[i]) {
      enemigoActivo[i] = true;
      enemigoX[i] = x;
      enemigoY[i] = y;
      enemigoTipo[i] = random(0, TOTAL_TIPOS_ENEMIGO);
      return true;
    }
  }

  return false;
}

void obtenerPatron(uint8_t patron, int8_t baseX, int8_t xs[], uint8_t ys[], uint8_t &cantidad) {
  cantidad = 0;

  if (patron == 0) {
    xs[0] = baseX; ys[0] = 1;
    xs[1] = baseX; ys[1] = 2;
    xs[2] = baseX; ys[2] = 3;
    cantidad = 3;
  }
  else if (patron == 1) {
    xs[0] = baseX; ys[0] = 1;
    xs[1] = baseX; ys[1] = 2;
    cantidad = 2;
  }
  else if (patron == 2) {
    xs[0] = baseX; ys[0] = 2;
    xs[1] = baseX; ys[1] = 3;
    cantidad = 2;
  }
  else if (patron == 3) {
    xs[0] = baseX; ys[0] = 1;
    xs[1] = baseX; ys[1] = 3;
    cantidad = 2;
  }
  else if (patron == 4) {
    xs[0] = baseX;
    ys[0] = random(FILA_JUEGO_MIN, FILA_JUEGO_MAX + 1);
    cantidad = 1;
  }
  else if (patron == 5) {
    xs[0] = baseX;     ys[0] = 1;
    xs[1] = baseX - 2; ys[1] = 2;
    xs[2] = baseX - 4; ys[2] = 3;
    cantidad = 3;
  }
  else if (patron == 6) {
    xs[0] = baseX - 4; ys[0] = 1;
    xs[1] = baseX - 2; ys[1] = 2;
    xs[2] = baseX;     ys[2] = 3;
    cantidad = 3;
  }
  else if (patron == 7) {
    xs[0] = baseX;     ys[0] = 1;
    xs[1] = baseX - 3; ys[1] = 2;
    xs[2] = baseX;     ys[2] = 3;
    cantidad = 3;
  }
}

bool intentarCrearPatron(uint8_t patron, int8_t baseX) {
  int8_t xs[MAXIMO_ENEMIGOS_PATRON];
  uint8_t ys[MAXIMO_ENEMIGOS_PATRON];
  uint8_t cantidad = 0;

  obtenerPatron(patron, baseX, xs, ys, cantidad);

  if (contarEnemigosActivos() + cantidad > maximoEnemigosActivos) {
    return false;
  }

  for (uint8_t i = 0; i < cantidad; i++) {
    if (!posicionLibre(xs[i], ys[i])) {
      return false;
    }
  }

  for (uint8_t i = 0; i < cantidad; i++) {
    crearEnemigo(xs[i], ys[i]);
  }

  return true;
}

void generarOleada() {
  if (pantallaActual != PANTALLA_JUEGO || juegoPausado) {
    return;
  }

  if (millis() - ultimaOleadaMs < intervaloOleadaMs) {
    return;
  }

  if (contarEnemigosActivos() >= maximoEnemigosActivos) {
    return;
  }

  ultimaOleadaMs = millis();

  actualizarDificultad();

  uint8_t patron = random(0, indicePatronMaximo + 1);
  int8_t baseX = random(aparicionXMin, aparicionXMax + 1);

  if (intentarCrearPatron(patron, baseX)) {
    solicitarRedibujoJuego();
  }
}

void moverEnemigos() {
  if (pantallaActual != PANTALLA_JUEGO || juegoPausado) {
    return;
  }

  if (millis() - ultimoMovimientoEnemigoMs < velocidadEnemigoMs) {
    return;
  }

  ultimoMovimientoEnemigoMs = millis();

  bool huboCambio = false;

  for (uint8_t i = 0; i < MAXIMO_ENEMIGOS; i++) {
    if (enemigoActivo[i]) {
      enemigoX[i]--;

      if (enemigoX[i] == jugadorX && enemigoY[i] == jugadorY) {
        enemigoActivo[i] = false;
        perderVida();
        huboCambio = true;

        if (pantallaActual != PANTALLA_JUEGO) {
          return;
        }

        continue;
      }

      if (enemigoX[i] < COLUMNA_MIN) {
        enemigoActivo[i] = false;
        perderVida();
        huboCambio = true;

        if (pantallaActual != PANTALLA_JUEGO) {
          return;
        }

        continue;
      }

      huboCambio = true;
    }
  }

  for (uint8_t fila = 0; fila < TOTAL_BALAS; fila++) {
    verificarColisionBala(fila);
  }

  verificarColisionEspecial();
  verificarChoqueJugador();

  if (huboCambio && pantallaActual == PANTALLA_JUEGO) {
    solicitarRedibujoJuego();
  }
}
