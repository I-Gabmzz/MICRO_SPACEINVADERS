#ifndef I2C_JUEGO_AVR_H
#define I2C_JUEGO_AVR_H

#include "AVRCompat.h"

void i2cJuegoInicializar();
bool i2cJuegoEscribir(uint8_t direccion, uint8_t dato);

#endif
