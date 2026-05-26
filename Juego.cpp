#include "Juego.h"
#include "CaracteresLCD.h"
#include "Disparos.h"
#include "Enemigos.h"
#include "Entrada.h"
#include "Jugador.h"
#include "Marcadores.h"
#include "Pantallas.h"
#include "Puntaje.h"
#include "Sonido.h"
#include "USARTAdmin.h"

void inicializarJuego() {
  lcd.init();
  lcd.backlight();

  cargarCaracteresLCD();

  Serial.begin(VELOCIDAD_SERIAL);

  pinMode(BOTON_ARRIBA, INPUT_PULLUP);
  pinMode(BOTON_ABAJO, INPUT_PULLUP);
  pinMode(BOTON_IZQUIERDA, INPUT_PULLUP);
  pinMode(BOTON_DERECHA, INPUT_PULLUP);
  pinMode(BOTON_DISPARO_1, INPUT_PULLUP);
  pinMode(BOTON_DISPARO_2, INPUT_PULLUP);

  pinMode(BUZZER_MUSICA, OUTPUT);
  pinMode(BUZZER_EFECTO, OUTPUT);

  randomSeed(analogRead(A0));

  comandoSerial[0] = '\0';

  for (uint8_t i = 0; i < TOTAL_INDICES_BOTON; i++) {
    ultimoCambioBotonMs[i] = 0;
  }

  invalidarCacheJuego();
  cargarMarcadores();
  mostrarInicio();

  Serial.println(F("USART ADMIN LISTO. Escribe HELP."));
}

void cicloJuego() {
  actualizarEfecto();
  actualizarMusica();
  leerUSARTAdmin();

  if (pantallaActual == PANTALLA_INICIO) {
    manejarInicio();
  }
  else if (pantallaActual == PANTALLA_MENU) {
    manejarMenu();
  }
  else if (pantallaActual == PANTALLA_MARCADORES) {
    manejarMarcadores();
  }
  else if (pantallaActual == PANTALLA_JUEGO) {
    manejarJuego();
  }
  else if (pantallaActual == PANTALLA_NUEVA_MARCA) {
    manejarNuevaMarca();
  }
  else if (pantallaActual == PANTALLA_FIN_PARTIDA) {
    manejarFinPartida();
  }

  procesarRedibujoPendiente();
}

void iniciarPartida() {
  pantallaActual = PANTALLA_JUEGO;
  juegoPausado = false;
  areaJuegoPendiente = false;
  invalidarCacheJuego();

  lcd.noBlink();

  puntaje = 0;
  vidas = VIDAS_INICIALES;
  nivel = NIVEL_INICIAL;
  jugadorX = COLUMNA_MIN;
  jugadorY = 2;

  ultimoDanioMs = 0;
  ultimoMovimientoEnemigoMs = millis();
  ultimaOleadaMs = millis();
  ultimaActualizacionBarraMs = 0;

  proyectilEspecialActivo = false;
  explosionActiva = false;
  especialUsadoUnaVez = false;
  ultimoEspecialMs = 0;

  efectoActivo = false;
  noTone(BUZZER_EFECTO);

  for (uint8_t fila = 0; fila < TOTAL_BALAS; fila++) {
    balaActiva[fila] = false;
    balaX[fila] = 0;
    balaY[fila] = fila + FILA_JUEGO_MIN;
    ultimoMovimientoBalaMs[fila] = millis();
  }

  inicializarEnemigos();
  actualizarDificultad();
  dibujarPantallaJuego();
}

void manejarJuego() {
  if (juegoPausado) {
    return;
  }

  if (botonPresionado(BOTON_ARRIBA, ultimoArriba)) {
    moverJugador(0, -1);
  }

  if (botonPresionado(BOTON_ABAJO, ultimoAbajo)) {
    moverJugador(0, 1);
  }

  if (botonPresionado(BOTON_IZQUIERDA, ultimoIzquierda)) {
    moverJugador(-1, 0);
  }

  if (botonPresionado(BOTON_DERECHA, ultimoDerecha)) {
    moverJugador(1, 0);
  }

  if (botonPresionado(BOTON_DISPARO_1, ultimoDisparo1)) {
    dispararBalaNormal();
  }

  if (botonPresionado(BOTON_DISPARO_2, ultimoDisparo2)) {
    dispararEspecial();
  }

  actualizarBalas();
  actualizarEspecial();
  actualizarExplosion();

  generarOleada();
  moverEnemigos();

  actualizarBarraEstado(false);
}
