#include "CaracteresLCD.h"

const byte DIBUJO_JUGADOR[] PROGMEM = {
  0b10000,
  0b11100,
  0b10110,
  0b11111,
  0b10110,
  0b11100,
  0b10000,
  0b00000
};

const byte DIBUJO_ENEMIGO_1[] PROGMEM = {
  0b10101,
  0b01110,
  0b11111,
  0b10101,
  0b11111,
  0b01010,
  0b10001,
  0b00000
};

const byte DIBUJO_BALA[] PROGMEM = {
  0b00000,
  0b00000,
  0b00100,
  0b11110,
  0b00100,
  0b00000,
  0b00000,
  0b00000
};

const byte DIBUJO_ESPECIAL[] PROGMEM = {
  0b00000,
  0b01100,
  0b11110,
  0b01111,
  0b11110,
  0b01100,
  0b00000,
  0b00000
};

const byte DIBUJO_EXPLOSION[] PROGMEM = {
  0b00100,
  0b10101,
  0b01110,
  0b11111,
  0b01110,
  0b10101,
  0b00100,
  0b00000
};

const byte DIBUJO_ENEMIGO_2[] PROGMEM = {
  0b01110,
  0b11111,
  0b10101,
  0b11111,
  0b00100,
  0b01010,
  0b10001,
  0b00000
};

const byte DIBUJO_ENEMIGO_3[] PROGMEM = {
  0b00011,
  0b00111,
  0b11110,
  0b11000,
  0b11110,
  0b00111,
  0b00011,
  0b00000
};

const byte DIBUJO_SELECTOR[] PROGMEM = {
  0b10000,
  0b11000,
  0b10100,
  0b10110,
  0b10100,
  0b11000,
  0b10000,
  0b00000
};

void cargarCaracteresLCD() {
  cargarCaracter(CAR_JUGADOR, DIBUJO_JUGADOR);
  cargarCaracter(CAR_ENEMIGO_1, DIBUJO_ENEMIGO_1);
  cargarCaracter(CAR_BALA, DIBUJO_BALA);
  cargarCaracter(CAR_ESPECIAL, DIBUJO_ESPECIAL);
  cargarCaracter(CAR_EXPLOSION, DIBUJO_EXPLOSION);
  cargarCaracter(CAR_ENEMIGO_2, DIBUJO_ENEMIGO_2);
  cargarCaracter(CAR_ENEMIGO_3, DIBUJO_ENEMIGO_3);
  cargarCaracter(CAR_SELECTOR, DIBUJO_SELECTOR);
}

void cargarCaracter(uint8_t posicion, const byte dibujo[]) {
  byte buffer[ALTURA_CARACTER_LCD];

  for (uint8_t i = 0; i < ALTURA_CARACTER_LCD; i++) {
    buffer[i] = pgm_read_byte(&dibujo[i]);
  }

  lcd.createChar(posicion, buffer);
}
