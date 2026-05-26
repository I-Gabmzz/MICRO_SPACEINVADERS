#include "TemporizadorJuego.h"
#include "AudioJuegoAVR.h"

volatile unsigned long milisegundosSistema = 0;

void temporizadorInicializar() {
  TCCR0A = (1 << WGM01);
  TCCR0B = (1 << CS01) | (1 << CS00);
  OCR0A = 249;
  TIMSK0 = (1 << OCIE0A);
}

unsigned long temporizadorMillis() {
  unsigned long copia;

  uint8_t sreg = SREG;
  cli();
  copia = milisegundosSistema;
  SREG = sreg;

  return copia;
}

ISR(TIMER0_COMPA_vect) {
  milisegundosSistema++;
  audioActualizarDesdeISR(milisegundosSistema);
}
