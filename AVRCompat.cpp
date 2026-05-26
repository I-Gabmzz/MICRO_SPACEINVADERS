#include "AVRCompat.h"
#include "AudioJuegoAVR.h"
#include "TemporizadorJuego.h"
#include "I2CJuegoAVR.h"

SerialAVR Serial;
EEPROMClass EEPROM;

static uint32_t estadoAzar = 0xA5A5A5A5UL;

static void escribirPin(uint8_t pin, bool salida, bool alto) {
  volatile uint8_t *ddr = NULL;
  volatile uint8_t *port = NULL;
  uint8_t bit = 0;

  if (pin <= 7) {
    ddr = &DDRD;
    port = &PORTD;
    bit = pin;
  }
  else if (pin <= 13) {
    ddr = &DDRB;
    port = &PORTB;
    bit = pin - 8;
  }
  else if (pin <= 19) {
    ddr = &DDRC;
    port = &PORTC;
    bit = pin - 14;
  }
  else {
    return;
  }

  if (salida) {
    *ddr |= _BV(bit);
  }
  else {
    *ddr &= ~_BV(bit);
  }

  if (alto) {
    *port |= _BV(bit);
  }
  else {
    *port &= ~_BV(bit);
  }
}

void sistemaInicializar() {
  temporizadorInicializar();
  audioInicializar();

  ADMUX = (1 << REFS0);
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  sei();
}

unsigned long millis() {
  return temporizadorMillis();
}

void delay(unsigned long ms) {
  unsigned long inicio = millis();
  while (millis() - inicio < ms) {
  }
}

void pinMode(uint8_t pin, uint8_t modo) {
  if (modo == OUTPUT) {
    escribirPin(pin, true, false);
  }
  else if (modo == INPUT_PULLUP) {
    escribirPin(pin, false, true);
  }
  else {
    escribirPin(pin, false, false);
  }
}

void digitalWrite(uint8_t pin, uint8_t valor) {
  escribirPin(pin, true, valor == HIGH);
}

uint16_t analogRead(uint8_t pin) {
  uint8_t canal = pin;
  if (pin >= A0) {
    canal = pin - A0;
  }

  ADMUX = (1 << REFS0) | (canal & 0x07);
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC)) {
  }

  return ADC;
}

void randomSeed(unsigned long semilla) {
  if (semilla != 0) {
    estadoAzar = semilla;
  }
}

long random(long maximo) {
  return random(0, maximo);
}

long random(long minimo, long maximo) {
  if (maximo <= minimo) {
    return minimo;
  }

  estadoAzar ^= estadoAzar << 13;
  estadoAzar ^= estadoAzar >> 17;
  estadoAzar ^= estadoAzar << 5;

  return minimo + (estadoAzar % (maximo - minimo));
}

void tone(uint8_t pin, uint16_t frecuencia) {
  audioTono(pin, frecuencia, 0);
}

void tone(uint8_t pin, uint16_t frecuencia, uint16_t duracionMs) {
  audioTono(pin, frecuencia, duracionMs);
}

void noTone(uint8_t pin) {
  audioSinTono(pin);
}

// ==========================
// LCD I2C PCF8574
// ==========================

#define LCD_RS 0x01
#define LCD_RW 0x02
#define LCD_EN 0x04
#define LCD_BACKLIGHT 0x08

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_FUNCTIONSET 0x20
#define LCD_SETDDRAMADDR 0x80
#define LCD_SETCGRAMADDR 0x40

#define LCD_ENTRYLEFT 0x02
#define LCD_2LINE 0x08
#define LCD_5x8DOTS 0x00
#define LCD_DISPLAYON 0x04
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t direccion, uint8_t columnas, uint8_t filas) {
  _direccion = direccion;
  _columnas = columnas;
  _filas = filas;
  _backlight = LCD_BACKLIGHT;
  _displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
}

void LiquidCrystal_I2C::init() {
  i2cJuegoInicializar();
  _delay_ms(50);

  escribir4Bits(0x30);
  _delay_ms(5);
  escribir4Bits(0x30);
  _delay_us(150);
  escribir4Bits(0x30);
  escribir4Bits(0x20);

  comando(LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS);
  comando(LCD_DISPLAYCONTROL | _displayControl);
  clear();
  comando(LCD_ENTRYMODESET | LCD_ENTRYLEFT);
}

void LiquidCrystal_I2C::backlight() {
  _backlight = LCD_BACKLIGHT;
  i2cJuegoEscribir(_direccion, _backlight);
}

void LiquidCrystal_I2C::clear() {
  comando(LCD_CLEARDISPLAY);
  _delay_ms(2);
}

void LiquidCrystal_I2C::setCursor(uint8_t columna, uint8_t fila) {
  static const uint8_t offsets[] = {0x00, 0x40, 0x14, 0x54};

  if (fila >= _filas) {
    fila = _filas - 1;
  }

  comando(LCD_SETDDRAMADDR | (columna + offsets[fila]));
}

void LiquidCrystal_I2C::createChar(uint8_t posicion, byte dibujo[]) {
  posicion &= 0x07;
  comando(LCD_SETCGRAMADDR | (posicion << 3));

  for (uint8_t i = 0; i < 8; i++) {
    write(dibujo[i]);
  }
}

void LiquidCrystal_I2C::write(uint8_t valor) {
  enviar(valor, LCD_RS);
}

void LiquidCrystal_I2C::print(const char *texto) {
  while (*texto) {
    write(*texto++);
  }
}

void LiquidCrystal_I2C::print(const __FlashStringHelper *texto) {
  const char *ptr = reinterpret_cast<const char *>(texto);
  char c = pgm_read_byte(ptr++);

  while (c) {
    write(c);
    c = pgm_read_byte(ptr++);
  }
}

void LiquidCrystal_I2C::print(char valor) {
  write(valor);
}

void LiquidCrystal_I2C::print(int valor) {
  if (valor < 0) {
    write('-');
    valor = -valor;
  }
  imprimirNumero(valor);
}

void LiquidCrystal_I2C::print(unsigned int valor) {
  imprimirNumero(valor);
}

void LiquidCrystal_I2C::print(long valor) {
  if (valor < 0) {
    write('-');
    valor = -valor;
  }
  imprimirNumero(valor);
}

void LiquidCrystal_I2C::print(unsigned long valor) {
  imprimirNumero(valor);
}

void LiquidCrystal_I2C::noBlink() {
  _displayControl &= ~LCD_BLINKON;
  comando(LCD_DISPLAYCONTROL | _displayControl);
}

void LiquidCrystal_I2C::blink() {
  _displayControl |= LCD_BLINKON;
  comando(LCD_DISPLAYCONTROL | _displayControl);
}

void LiquidCrystal_I2C::enviar(uint8_t valor, uint8_t modo) {
  escribir4Bits((valor & 0xF0) | modo);
  escribir4Bits(((valor << 4) & 0xF0) | modo);
}

void LiquidCrystal_I2C::comando(uint8_t valor) {
  enviar(valor, 0);
}

void LiquidCrystal_I2C::escribir4Bits(uint8_t valor) {
  uint8_t dato = valor | _backlight;
  i2cJuegoEscribir(_direccion, dato);
  pulsoEnable(dato);
}

void LiquidCrystal_I2C::pulsoEnable(uint8_t valor) {
  i2cJuegoEscribir(_direccion, valor | LCD_EN);
  _delay_us(1);
  i2cJuegoEscribir(_direccion, valor & ~LCD_EN);
  _delay_us(50);
}

void LiquidCrystal_I2C::imprimirNumero(unsigned long valor) {
  char buffer[11];
  ultoa(valor, buffer, 10);
  print(buffer);
}

// ==========================
// USART
// ==========================

void SerialAVR::begin(unsigned long velocidad) {
  uint16_t ubrr = (F_CPU / 16UL / velocidad) - 1;

  UBRR0H = (uint8_t)(ubrr >> 8);
  UBRR0L = (uint8_t)ubrr;
  UCSR0A = 0;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

int SerialAVR::available() {
  return (UCSR0A & (1 << RXC0)) ? 1 : 0;
}

char SerialAVR::read() {
  return UDR0;
}

void SerialAVR::escribir(char valor) {
  while (!(UCSR0A & (1 << UDRE0))) {
  }
  UDR0 = valor;
}

void SerialAVR::print(const char *texto) {
  while (*texto) {
    escribir(*texto++);
  }
}

void SerialAVR::print(const __FlashStringHelper *texto) {
  const char *ptr = reinterpret_cast<const char *>(texto);
  char c = pgm_read_byte(ptr++);

  while (c) {
    escribir(c);
    c = pgm_read_byte(ptr++);
  }
}

void SerialAVR::print(char valor) {
  escribir(valor);
}

void SerialAVR::print(int valor) {
  if (valor < 0) {
    escribir('-');
    valor = -valor;
  }
  imprimirNumero(valor);
}

void SerialAVR::print(unsigned int valor) {
  imprimirNumero(valor);
}

void SerialAVR::print(long valor) {
  if (valor < 0) {
    escribir('-');
    valor = -valor;
  }
  imprimirNumero(valor);
}

void SerialAVR::print(unsigned long valor) {
  imprimirNumero(valor);
}

void SerialAVR::println() {
  escribir('\r');
  escribir('\n');
}

void SerialAVR::println(const char *texto) {
  print(texto);
  println();
}

void SerialAVR::println(const __FlashStringHelper *texto) {
  print(texto);
  println();
}

void SerialAVR::println(int valor) {
  print(valor);
  println();
}

void SerialAVR::println(unsigned int valor) {
  print(valor);
  println();
}

void SerialAVR::println(long valor) {
  print(valor);
  println();
}

void SerialAVR::println(unsigned long valor) {
  print(valor);
  println();
}

void SerialAVR::imprimirNumero(unsigned long valor) {
  char buffer[11];
  ultoa(valor, buffer, 10);
  print(buffer);
}

// ==========================
// EEPROM
// ==========================

uint8_t EEPROMClass::read(int direccion) {
  return eeprom_read_byte((const uint8_t *)direccion);
}

void EEPROMClass::update(int direccion, uint8_t valor) {
  eeprom_update_byte((uint8_t *)direccion, valor);
}
