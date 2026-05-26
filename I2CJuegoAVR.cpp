#include "I2CJuegoAVR.h"

static bool i2cJuegoEsperar() {
  while (!(TWCR & (1 << TWINT))) {
  }
  return true;
}

void i2cJuegoInicializar() {
  TWSR = 0;
  TWBR = ((F_CPU / 100000UL) - 16) / 2;
  TWCR = (1 << TWEN);
}

bool i2cJuegoEscribir(uint8_t direccion, uint8_t dato) {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  i2cJuegoEsperar();

  TWDR = (direccion << 1);
  TWCR = (1 << TWINT) | (1 << TWEN);
  i2cJuegoEsperar();

  TWDR = dato;
  TWCR = (1 << TWINT) | (1 << TWEN);
  i2cJuegoEsperar();

  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
  return true;
}
