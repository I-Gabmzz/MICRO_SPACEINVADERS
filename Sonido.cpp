#include "Sonido.h"

const uint16_t frecuenciasMusica[] PROGMEM = {
  196, 0, 196, 247, 196, 0, 233, 0,
  196, 0, 196, 262, 196, 0, 294, 0,
  247, 0, 247, 294, 247, 0, 262, 0,
  233, 0, 220, 0, 196, 0, 165, 0,

  196, 247, 294, 247, 196, 0, 165, 0,
  220, 262, 330, 262, 220, 0, 196, 0,
  247, 294, 349, 294, 247, 0, 220, 0,
  196, 165, 147, 165, 196, 0, 0, 0
};

const uint16_t duracionesMusica[] PROGMEM = {
  120, 45, 120, 95, 120, 45, 130, 65,
  120, 45, 120, 95, 120, 45, 150, 80,
  120, 45, 120, 95, 120, 45, 130, 65,
  120, 45, 120, 45, 150, 60, 190, 90,

  95, 95, 120, 95, 150, 60, 150, 70,
  95, 95, 120, 95, 150, 60, 150, 70,
  95, 95, 120, 95, 150, 60, 150, 70,
  100, 100, 120, 100, 180, 70, 90, 90
};

const uint8_t TOTAL_NOTAS_MUSICA = sizeof(frecuenciasMusica) / sizeof(frecuenciasMusica[0]);

const uint16_t FRECUENCIAS_DISPARO[] PROGMEM = {2400, 1900, 1300};
const uint16_t DURACIONES_DISPARO[]  PROGMEM = {25, 30, 35};
const uint16_t FRECUENCIAS_EXPLOSION[] PROGMEM = {420, 260, 150, 90};
const uint16_t DURACIONES_EXPLOSION[]  PROGMEM = {45, 55, 70, 100};
const uint16_t FRECUENCIAS_DANIO[] PROGMEM = {392, 294, 220, 147};
const uint16_t DURACIONES_DANIO[]  PROGMEM = {55, 65, 75, 110};
const uint16_t FRECUENCIAS_ERROR[] PROGMEM = {180, 0, 180};
const uint16_t DURACIONES_ERROR[]  PROGMEM = {70, 35, 90};
const uint16_t FRECUENCIAS_ESPECIAL[] PROGMEM = {523, 784, 1047, 1568};
const uint16_t DURACIONES_ESPECIAL[]  PROGMEM = {45, 45, 55, 80};
const uint16_t FRECUENCIAS_FIN_PARTIDA[] PROGMEM = {392, 330, 262, 196, 131, 98};
const uint16_t DURACIONES_FIN_PARTIDA[]  PROGMEM = {120, 120, 150, 180, 220, 320};

void alternarMusica() {
  musicaActiva = !musicaActiva;

  if (!musicaActiva) {
    detenerMusica();
  }
}

void detenerMusica() {
  if (notaMusicaIniciada) {
    noTone(BUZZER_MUSICA);
  }

  notaMusicaIniciada = false;
}

void iniciarNotaMusicaActual() {
  uint16_t frecuencia = pgm_read_word(&frecuenciasMusica[indiceMusica]);
  uint16_t duracion = obtenerDuracionMusicaActual();
  uint16_t duracionTono = (duracion * PORCENTAJE_TONO_MUSICA) / PORCENTAJE_DURACION_BASE;

  if (frecuencia == 0) {
    noTone(BUZZER_MUSICA);
  }
  else {
    tone(BUZZER_MUSICA, frecuencia, duracionTono);
  }

  inicioNotaMusicaMs = millis();
  notaMusicaIniciada = true;
}

uint16_t obtenerDuracionMusicaActual() {
  uint16_t duracion = pgm_read_word(&duracionesMusica[indiceMusica]);

  if (pantallaActual == PANTALLA_JUEGO && nivel > NIVEL_INICIAL) {
    uint8_t reduccion = (nivel - NIVEL_INICIAL) * REDUCCION_MUSICA_POR_NIVEL;
    duracion = (duracion * (PORCENTAJE_DURACION_BASE - reduccion)) / PORCENTAJE_DURACION_BASE;
  }

  if (duracion < DURACION_MINIMA_NOTA_MS) {
    duracion = DURACION_MINIMA_NOTA_MS;
  }

  return duracion;
}

void actualizarMusica() {
  if (!musicaActiva) {
    detenerMusica();
    return;
  }

  // tone() en Arduino UNO solo permite un tono a la vez.
  if (efectoActivo) {
    return;
  }

  if (!notaMusicaIniciada) {
    iniciarNotaMusicaActual();
    return;
  }

  uint16_t duracion = obtenerDuracionMusicaActual();

  if (millis() - inicioNotaMusicaMs >= duracion) {
    indiceMusica++;

    if (indiceMusica >= TOTAL_NOTAS_MUSICA) {
      indiceMusica = 0;
    }

    iniciarNotaMusicaActual();
  }
}

void iniciarNotaEfectoActual() {
  uint16_t frecuencia = pgm_read_word(frecuenciasEfecto + indiceEfecto);

  if (frecuencia == 0) {
    noTone(BUZZER_EFECTO);
  }
  else {
    tone(BUZZER_EFECTO, frecuencia);
  }

  inicioNotaEfectoMs = millis();
}

void iniciarEfecto(const uint16_t frecuencias[], const uint16_t duraciones[], uint8_t longitud) {
  frecuenciasEfecto = frecuencias;
  duracionesEfecto = duraciones;
  longitudEfecto = longitud;
  indiceEfecto = 0;
  efectoActivo = true;

  detenerMusica();
  iniciarNotaEfectoActual();
}

void actualizarEfecto() {
  if (!efectoActivo) {
    return;
  }

  uint16_t duracion = pgm_read_word(duracionesEfecto + indiceEfecto);

  if (millis() - inicioNotaEfectoMs < duracion) {
    return;
  }

  indiceEfecto++;

  if (indiceEfecto >= longitudEfecto) {
    efectoActivo = false;
    noTone(BUZZER_EFECTO);
    notaMusicaIniciada = false;
    return;
  }

  iniciarNotaEfectoActual();
}

void efectoDisparo() {
  iniciarEfecto(FRECUENCIAS_DISPARO, DURACIONES_DISPARO, sizeof(FRECUENCIAS_DISPARO) / sizeof(FRECUENCIAS_DISPARO[0]));
}

void efectoExplosion() {
  iniciarEfecto(FRECUENCIAS_EXPLOSION, DURACIONES_EXPLOSION, sizeof(FRECUENCIAS_EXPLOSION) / sizeof(FRECUENCIAS_EXPLOSION[0]));
}

void efectoDanio() {
  iniciarEfecto(FRECUENCIAS_DANIO, DURACIONES_DANIO, sizeof(FRECUENCIAS_DANIO) / sizeof(FRECUENCIAS_DANIO[0]));
}

void efectoError() {
  iniciarEfecto(FRECUENCIAS_ERROR, DURACIONES_ERROR, sizeof(FRECUENCIAS_ERROR) / sizeof(FRECUENCIAS_ERROR[0]));
}

void efectoEspecial() {
  iniciarEfecto(FRECUENCIAS_ESPECIAL, DURACIONES_ESPECIAL, sizeof(FRECUENCIAS_ESPECIAL) / sizeof(FRECUENCIAS_ESPECIAL[0]));
}

void efectoFinPartida() {
  iniciarEfecto(FRECUENCIAS_FIN_PARTIDA, DURACIONES_FIN_PARTIDA, sizeof(FRECUENCIAS_FIN_PARTIDA) / sizeof(FRECUENCIAS_FIN_PARTIDA[0]));
}
