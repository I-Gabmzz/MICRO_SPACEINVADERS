#ifndef DISPAROS_H
#define DISPAROS_H

#include "EstadoJuego.h"

void dispararBalaNormal();
void actualizarBalas();
void dispararEspecial();
void actualizarEspecial();
bool especialDisponible();
void activarExplosion(int8_t x, uint8_t y);
void actualizarExplosion();
void generarMascaraExplosion();
void aplicarDanioExplosion();

#endif
