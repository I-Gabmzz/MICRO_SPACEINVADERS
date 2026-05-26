#ifndef AUDIO_JUEGO_AVR_H
#define AUDIO_JUEGO_AVR_H

#include "AVRCompat.h"

void audioInicializar();
void audioTono(uint8_t pin, uint16_t frecuencia, uint16_t duracionMs);
void audioSinTono(uint8_t pin);
void audioActualizarDesdeISR(unsigned long ahoraMs);

#endif
