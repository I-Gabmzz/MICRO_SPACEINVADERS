#ifndef MARCADORES_H
#define MARCADORES_H

#include "EstadoJuego.h"

void cargarMarcadores();
void inicializarMarcadores();
void guardarMarcadores();
int8_t obtenerPosicionMarcador(uint16_t puntos);
void guardarNuevoMarcador();
void resetearMarcadores();

#endif
