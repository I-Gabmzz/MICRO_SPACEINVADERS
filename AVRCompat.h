#ifndef AVR_COMPAT_H
#define AVR_COMPAT_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef uint8_t byte;

#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define A0 14

class __FlashStringHelper;
#define F(texto) (reinterpret_cast<const __FlashStringHelper *>(PSTR(texto)))

void sistemaInicializar();

unsigned long millis();
void delay(unsigned long ms);

void pinMode(uint8_t pin, uint8_t modo);
void digitalWrite(uint8_t pin, uint8_t valor);
uint16_t analogRead(uint8_t pin);

void randomSeed(unsigned long semilla);
long random(long maximo);
long random(long minimo, long maximo);

void tone(uint8_t pin, uint16_t frecuencia);
void tone(uint8_t pin, uint16_t frecuencia, uint16_t duracionMs);
void noTone(uint8_t pin);

class LiquidCrystal_I2C {
public:
  LiquidCrystal_I2C(uint8_t direccion, uint8_t columnas, uint8_t filas);

  void init();
  void backlight();
  void clear();
  void setCursor(uint8_t columna, uint8_t fila);
  void createChar(uint8_t posicion, byte dibujo[]);
  void write(uint8_t valor);
  void print(const char *texto);
  void print(const __FlashStringHelper *texto);
  void print(char valor);
  void print(int valor);
  void print(unsigned int valor);
  void print(long valor);
  void print(unsigned long valor);
  void noBlink();
  void blink();

private:
  uint8_t _direccion;
  uint8_t _columnas;
  uint8_t _filas;
  uint8_t _backlight;
  uint8_t _displayControl;

  void enviar(uint8_t valor, uint8_t modo);
  void comando(uint8_t valor);
  void escribir4Bits(uint8_t valor);
  void pulsoEnable(uint8_t valor);
  void imprimirNumero(unsigned long valor);
};

class SerialAVR {
public:
  void begin(unsigned long velocidad);
  int available();
  char read();
  void print(const char *texto);
  void print(const __FlashStringHelper *texto);
  void print(char valor);
  void print(int valor);
  void print(unsigned int valor);
  void print(long valor);
  void print(unsigned long valor);
  void println();
  void println(const char *texto);
  void println(const __FlashStringHelper *texto);
  void println(int valor);
  void println(unsigned int valor);
  void println(long valor);
  void println(unsigned long valor);

private:
  void escribir(char valor);
  void imprimirNumero(unsigned long valor);
};

extern SerialAVR Serial;

class EEPROMClass {
public:
  uint8_t read(int direccion);
  void update(int direccion, uint8_t valor);

  template <typename T>
  void get(int direccion, T &valor) {
    eeprom_read_block((void *)&valor, (const void *)direccion, sizeof(T));
  }

  template <typename T>
  void put(int direccion, const T &valor) {
    eeprom_update_block((const void *)&valor, (void *)direccion, sizeof(T));
  }
};

extern EEPROMClass EEPROM;

#endif
