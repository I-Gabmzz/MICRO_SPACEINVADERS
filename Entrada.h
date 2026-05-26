#ifndef ENTRADA_H
#define ENTRADA_H

#include "EstadoJuego.h"

bool leerBotonDirecto(uint8_t pin);
bool botonPresionado(uint8_t pin, bool &estadoAnterior);

#endif
