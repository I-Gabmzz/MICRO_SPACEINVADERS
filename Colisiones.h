#ifndef COLISIONES_H
#define COLISIONES_H

#include "EstadoJuego.h"

int8_t buscarEnemigoEnPosicion(int8_t x, uint8_t y);
void verificarColisionBala(uint8_t fila);
void verificarColisionEspecial();
void verificarChoqueJugador();

#endif
