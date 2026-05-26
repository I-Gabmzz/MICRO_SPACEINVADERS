#include "AudioJuegoAVR.h"
#include "ConfigJuego.h"
#include "TemporizadorJuego.h"

static volatile bool musicaConDuracion = false;
static volatile bool efectoConDuracion = false;
static volatile unsigned long apagarMusicaEnMs = 0;
static volatile unsigned long apagarEfectoEnMs = 0;

static void configurarTimer1(uint16_t frecuencia) {
  if (frecuencia == 0) {
    TCCR1A &= ~(1 << COM1A0);
    PORTB &= ~(1 << PB1);
    return;
  }

  const uint16_t prescaladores[] = {1, 8, 64, 256, 1024};
  const uint16_t bits[] = {
    (1 << CS10),
    (1 << CS11),
    (1 << CS11) | (1 << CS10),
    (1 << CS12),
    (1 << CS12) | (1 << CS10)
  };

  uint16_t ocr = 0;
  uint16_t seleccionBits = bits[0];

  for (uint8_t i = 0; i < 5; i++) {
    unsigned long calculo = (F_CPU / (2UL * prescaladores[i] * frecuencia)) - 1;
    if (calculo <= 65535UL) {
      ocr = calculo;
      seleccionBits = bits[i];
      break;
    }
  }

  DDRB |= (1 << PB1);
  TCCR1A = (1 << COM1A0);
  TCCR1B = (1 << WGM12) | seleccionBits;
  OCR1A = ocr;
}

static void configurarTimer2(uint16_t frecuencia) {
  if (frecuencia == 0) {
    TCCR2A &= ~(1 << COM2A0);
    PORTB &= ~(1 << PB3);
    return;
  }

  const uint16_t prescaladores[] = {1, 8, 32, 64, 128, 256, 1024};
  const uint16_t bits[] = {
    (1 << CS20),
    (1 << CS21),
    (1 << CS21) | (1 << CS20),
    (1 << CS22),
    (1 << CS22) | (1 << CS20),
    (1 << CS22) | (1 << CS21),
    (1 << CS22) | (1 << CS21) | (1 << CS20)
  };

  uint8_t ocr = 255;
  uint16_t seleccionBits = bits[6];

  for (uint8_t i = 0; i < 7; i++) {
    unsigned long calculo = (F_CPU / (2UL * prescaladores[i] * frecuencia)) - 1;
    if (calculo <= 255UL) {
      ocr = calculo;
      seleccionBits = bits[i];
      break;
    }
  }

  DDRB |= (1 << PB3);
  TCCR2A = (1 << COM2A0) | (1 << WGM21);
  TCCR2B = seleccionBits;
  OCR2A = ocr;
}

void audioInicializar() {
  DDRB |= (1 << PB1) | (1 << PB3);
  configurarTimer1(0);
  configurarTimer2(0);
}

void audioTono(uint8_t pin, uint16_t frecuencia, uint16_t duracionMs) {
  uint8_t sreg = SREG;
  cli();

  if (pin == BUZZER_MUSICA) {
    configurarTimer1(frecuencia);
    musicaConDuracion = duracionMs > 0;
    apagarMusicaEnMs = temporizadorMillis() + duracionMs;
  }
  else if (pin == BUZZER_EFECTO) {
    configurarTimer2(frecuencia);
    efectoConDuracion = duracionMs > 0;
    apagarEfectoEnMs = temporizadorMillis() + duracionMs;
  }

  SREG = sreg;
}

void audioSinTono(uint8_t pin) {
  uint8_t sreg = SREG;
  cli();

  if (pin == BUZZER_MUSICA) {
    musicaConDuracion = false;
    configurarTimer1(0);
  }
  else if (pin == BUZZER_EFECTO) {
    efectoConDuracion = false;
    configurarTimer2(0);
  }

  SREG = sreg;
}

void audioActualizarDesdeISR(unsigned long ahoraMs) {
  if (musicaConDuracion && (long)(ahoraMs - apagarMusicaEnMs) >= 0) {
    musicaConDuracion = false;
    configurarTimer1(0);
  }

  if (efectoConDuracion && (long)(ahoraMs - apagarEfectoEnMs) >= 0) {
    efectoConDuracion = false;
    configurarTimer2(0);
  }
}
