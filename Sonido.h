#ifndef SONIDO_H
#define SONIDO_H

#include "EstadoJuego.h"

void alternarMusica();
void detenerMusica();
void iniciarNotaMusicaActual();
uint16_t obtenerDuracionMusicaActual();
void actualizarMusica();
void iniciarNotaEfectoActual();
void iniciarEfecto(const uint16_t frecuencias[], const uint16_t duraciones[], uint8_t longitud);
void actualizarEfecto();
void efectoDisparo();
void efectoExplosion();
void efectoDanio();
void efectoError();
void efectoEspecial();
void efectoFinPartida();

#endif
