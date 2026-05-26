#include "Colisiones.h"
#include "Disparos.h"
#include "Pantallas.h"
#include "Puntaje.h"
#include "Sonido.h"

int8_t buscarEnemigoEnPosicion(int8_t x, uint8_t y) {
  for (uint8_t i = 0; i < MAXIMO_ENEMIGOS; i++) {
    if (enemigoActivo[i]) {
      if (enemigoX[i] == x && enemigoY[i] == y) {
        return i;
      }
    }
  }

  return -1;
}

void verificarColisionBala(uint8_t fila) {
  if (!balaActiva[fila]) {
    return;
  }

  int8_t enemigo = buscarEnemigoEnPosicion(balaX[fila], balaY[fila]);

  if (enemigo != -1) {
    enemigoActivo[enemigo] = false;
    balaActiva[fila] = false;

    aumentarPuntaje(1);
    efectoExplosion();
    solicitarRedibujoJuego();
  }
}

void verificarColisionEspecial() {
  if (!proyectilEspecialActivo) {
    return;
  }

  int8_t enemigo = buscarEnemigoEnPosicion(especialX, especialY);

  if (enemigo != -1) {
    proyectilEspecialActivo = false;
    activarExplosion(especialX, especialY);
  }
}

void verificarChoqueJugador() {
  int8_t enemigo = buscarEnemigoEnPosicion(jugadorX, jugadorY);

  if (enemigo != -1) {
    enemigoActivo[enemigo] = false;
    perderVida();
    solicitarRedibujoJuego();
  }
}
