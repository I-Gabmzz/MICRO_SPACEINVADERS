#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include "EstadoJuego.h"

void inicializarEnemigos();
uint8_t contarEnemigosActivos();
bool posicionLibre(int8_t x, uint8_t y);
bool crearEnemigo(int8_t x, uint8_t y);
void obtenerPatron(uint8_t patron, int8_t baseX, int8_t xs[], uint8_t ys[], uint8_t &cantidad);
bool intentarCrearPatron(uint8_t patron, int8_t baseX);
void generarOleada();
void moverEnemigos();

#endif
