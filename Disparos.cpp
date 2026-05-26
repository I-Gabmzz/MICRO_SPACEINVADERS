#include "Disparos.h"
#include "Colisiones.h"
#include "Pantallas.h"
#include "Puntaje.h"
#include "Sonido.h"

void dispararBalaNormal() {
  uint8_t fila = jugadorY - FILA_JUEGO_MIN;

  if (balaActiva[fila]) {
    return;
  }

  if (jugadorX >= COLUMNA_MAX) {
    return;
  }

  balaActiva[fila] = true;
  balaX[fila] = jugadorX + 1;
  balaY[fila] = jugadorY;
  ultimoMovimientoBalaMs[fila] = millis();

  efectoDisparo();

  verificarColisionBala(fila);
  solicitarRedibujoJuego();
}

void actualizarBalas() {
  for (uint8_t fila = 0; fila < TOTAL_BALAS; fila++) {
    if (!balaActiva[fila]) {
      continue;
    }

    if (millis() - ultimoMovimientoBalaMs[fila] >= velocidadBalaMs) {
      ultimoMovimientoBalaMs[fila] = millis();

      balaX[fila]++;

      if (balaX[fila] > COLUMNA_MAX) {
        balaActiva[fila] = false;
      }
      else {
        verificarColisionBala(fila);
      }

      solicitarRedibujoJuego();
    }
  }
}

void dispararEspecial() {
  if (!especialDisponible()) {
    efectoError();
    return;
  }

  if (jugadorX >= COLUMNA_MAX - 1) {
    efectoError();
    return;
  }

  if (!modoAdmin) {
    especialUsadoUnaVez = true;
    ultimoEspecialMs = millis();
  }

  proyectilEspecialActivo = true;
  especialX = jugadorX + 1;
  especialY = jugadorY;
  ultimoMovimientoEspecialMs = millis();

  efectoEspecial();

  verificarColisionEspecial();
  solicitarRedibujoJuego();
}

void actualizarEspecial() {
  if (!proyectilEspecialActivo) {
    return;
  }

  if (millis() - ultimoMovimientoEspecialMs >= velocidadEspecialMs) {
    ultimoMovimientoEspecialMs = millis();

    especialX++;

    if (especialX >= COLUMNA_MAX - 1) {
      proyectilEspecialActivo = false;
      activarExplosion(especialX, especialY);
      return;
    }

    verificarColisionEspecial();
    solicitarRedibujoJuego();
  }
}

bool especialDisponible() {
  if (modoAdmin) {
    return !proyectilEspecialActivo && !explosionActiva;
  }

  if (!especialUsadoUnaVez) {
    return true;
  }

  if (proyectilEspecialActivo || explosionActiva) {
    return false;
  }

  return millis() - ultimoEspecialMs >= recargaEspecialMs;
}

void activarExplosion(int8_t x, uint8_t y) {
  explosionActiva = true;
  explosionX = x;
  explosionY = y;
  inicioExplosionMs = millis();

  generarMascaraExplosion();
  aplicarDanioExplosion();

  efectoExplosion();
  solicitarRedibujoJuego();
}

void actualizarExplosion() {
  if (!explosionActiva) {
    return;
  }

  if (millis() - inicioExplosionMs >= duracionExplosionMs) {
    explosionActiva = false;
    solicitarRedibujoJuego();
  }
}

void generarMascaraExplosion() {
  for (uint8_t fy = 0; fy < FILAS_JUEGO; fy++) {
    for (uint8_t fx = 0; fx < FILAS_JUEGO; fx++) {
      mascaraExplosion[fy][fx] = false;
    }
  }

  for (int8_t dy = -1; dy <= 1; dy++) {
    for (int8_t dx = -1; dx <= 1; dx++) {
      bool mostrar = false;

      if (dx == 0 && dy == 0) {
        mostrar = true;
      }
      else if (dx == 0 || dy == 0) {
        mostrar = random(0, 100) < 60;
      }
      else {
        mostrar = random(0, 100) < 35;
      }

      mascaraExplosion[dy + 1][dx + 1] = mostrar;
    }
  }
}

void aplicarDanioExplosion() {
  uint8_t eliminados = 0;

  for (uint8_t i = 0; i < MAXIMO_ENEMIGOS; i++) {
    if (!enemigoActivo[i]) {
      continue;
    }

    int8_t dx = enemigoX[i] - explosionX;
    int8_t dy = enemigoY[i] - explosionY;

    if (dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1) {
      if (mascaraExplosion[dy + 1][dx + 1]) {
        enemigoActivo[i] = false;
        eliminados++;
      }
    }
  }

  if (eliminados > 0) {
    aumentarPuntaje(eliminados);
  }
}
