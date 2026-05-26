#include "Entrada.h"

bool leerBotonDirecto(uint8_t pin) {
  return (PIND & _BV(pin)) ? HIGH : LOW;
}

bool botonPresionado(uint8_t pin, bool &estadoAnterior) {
  bool estadoActual = leerBotonDirecto(pin);
  bool presionado = false;

  if (estadoAnterior == HIGH && estadoActual == LOW) {
    if (millis() - ultimoCambioBotonMs[pin] >= tiempoReboteMs) {
      presionado = true;
      ultimoCambioBotonMs[pin] = millis();
    }
  }

  if (estadoAnterior == LOW && estadoActual == HIGH) {
    ultimoCambioBotonMs[pin] = millis();
  }

  estadoAnterior = estadoActual;
  return presionado;
}
