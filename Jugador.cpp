#include "Jugador.h"
#include "Colisiones.h"
#include "Pantallas.h"

void moverJugador(int8_t dx, int8_t dy) {
  int8_t nuevoX = jugadorX + dx;
  int8_t nuevoY = jugadorY + dy;

  if (nuevoX < COLUMNA_MIN) nuevoX = COLUMNA_MIN;
  if (nuevoX > COLUMNA_MAX) nuevoX = COLUMNA_MAX;

  if (nuevoY < FILA_JUEGO_MIN) nuevoY = FILA_JUEGO_MIN;
  if (nuevoY > FILA_JUEGO_MAX) nuevoY = FILA_JUEGO_MAX;

  jugadorX = nuevoX;
  jugadorY = nuevoY;

  verificarChoqueJugador();
  solicitarRedibujoJuego();
}
