#include "Pantallas.h"
#include "Disparos.h"
#include "Entrada.h"
#include "Juego.h"
#include "Marcadores.h"
#include "Sonido.h"

void limpiarFila(uint8_t fila) {
  lcd.setCursor(0, fila);
  lcd.print(F("                    "));
}

void imprimirPuntajeTresDigitos(uint16_t valor) {
  if (valor < 100) lcd.print(F("0"));
  if (valor < 10) lcd.print(F("0"));
  lcd.print(valor);
}

void invalidarCacheJuego() {
  for (uint8_t fila = 0; fila < FILAS_JUEGO; fila++) {
    for (uint8_t col = 0; col < COLUMNAS_LCD; col++) {
      cacheJuego[fila][col] = VALOR_CACHE_INVALIDO;
    }
  }
}

void solicitarRedibujoJuego() {
  if (pantallaActual == PANTALLA_JUEGO && !juegoPausado) {
    areaJuegoPendiente = true;
  }
}

void procesarRedibujoPendiente() {
  if (areaJuegoPendiente && pantallaActual == PANTALLA_JUEGO && !juegoPausado) {
    redibujarAreaJuego();
    areaJuegoPendiente = false;
  }
}

void mostrarInicio() {
  pantallaActual = PANTALLA_INICIO;
  juegoPausado = false;
  areaJuegoPendiente = false;
  invalidarCacheJuego();

  lcd.noBlink();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(F("+==================+"));

  lcd.setCursor(0, 1);
  lcd.print(F("|  SPACE INVADERS  |"));

  lcd.setCursor(0, 2);
  lcd.print(F("|  PRESIONA START  |"));

  lcd.setCursor(0, 3);
  lcd.print(F("+==================+"));
}

void manejarInicio() {
  if (botonPresionado(BOTON_DISPARO_1, ultimoDisparo1)) {
    mostrarMenu();
  }
}

void mostrarMenu() {
  pantallaActual = PANTALLA_MENU;
  juegoPausado = false;
  areaJuegoPendiente = false;
  invalidarCacheJuego();

  lcd.noBlink();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(F("MENU"));

  imprimirOpcionMenu(1, F("1. JUGAR"), opcionMenu == 0);
  imprimirOpcionMenu(2, F("2. PUNTAJES"), opcionMenu == 1);

  if (musicaActiva) {
    imprimirOpcionMenu(3, F("3. MUSICA: ON"), opcionMenu == ULTIMA_OPCION_MENU);
  } else {
    imprimirOpcionMenu(3, F("3. MUSICA: OFF"), opcionMenu == ULTIMA_OPCION_MENU);
  }
}

void imprimirOpcionMenu(uint8_t fila, const __FlashStringHelper *texto, bool seleccionada) {
  limpiarFila(fila);

  if (seleccionada) {
    lcd.setCursor(0, fila);
    lcd.write((uint8_t)CAR_SELECTOR);
  }

  uint8_t inicio = 2;

  if (fila == 1) inicio = 6;
  else if (fila == 2) inicio = 4;
  else if (fila == 3) inicio = 3;

  lcd.setCursor(inicio, fila);
  lcd.print(texto);
}

void manejarMenu() {
  if (botonPresionado(BOTON_ARRIBA, ultimoArriba)) {
    if (opcionMenu == 0) opcionMenu = ULTIMA_OPCION_MENU;
    else opcionMenu--;

    mostrarMenu();
  }

  if (botonPresionado(BOTON_ABAJO, ultimoAbajo)) {
    opcionMenu++;
    if (opcionMenu >= TOTAL_OPCIONES_MENU) opcionMenu = 0;

    mostrarMenu();
  }

  if (botonPresionado(BOTON_IZQUIERDA, ultimoIzquierda) || botonPresionado(BOTON_DERECHA, ultimoDerecha)) {
    if (opcionMenu == ULTIMA_OPCION_MENU) {
      alternarMusica();
      mostrarMenu();
    }
  }

  if (botonPresionado(BOTON_DISPARO_1, ultimoDisparo1)) {
    if (opcionMenu == 0) {
      iniciarPartida();
    }
    else if (opcionMenu == 1) {
      mostrarMarcadores();
    }
    else if (opcionMenu == ULTIMA_OPCION_MENU) {
      alternarMusica();
      mostrarMenu();
    }
  }

  if (botonPresionado(BOTON_DISPARO_2, ultimoDisparo2)) {
    mostrarInicio();
  }
}

void mostrarMarcadores() {
  pantallaActual = PANTALLA_MARCADORES;
  areaJuegoPendiente = false;
  invalidarCacheJuego();

  lcd.noBlink();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(F("+==== PUNTAJES ====+"));

  for (uint8_t i = 0; i < TOTAL_MARCADORES; i++) {
    lcd.setCursor(0, i + 1);
    lcd.print(F("| "));

    lcd.print(i + 1);
    lcd.print(F(" "));
    lcd.print(marcadores[i].nombre);
    lcd.print(F(" "));

    imprimirPuntajeTresDigitos(marcadores[i].puntos);

    lcd.print(F("        |"));
  }
}

void manejarMarcadores() {
  if (botonPresionado(BOTON_DISPARO_1, ultimoDisparo1) || botonPresionado(BOTON_DISPARO_2, ultimoDisparo2)) {
    mostrarMenu();
  }
}

void mostrarNuevaMarca() {
  pantallaActual = PANTALLA_NUEVA_MARCA;
  areaJuegoPendiente = false;
  invalidarCacheJuego();

  lcd.clear();
  lcd.blink();

  lcd.setCursor(0, 0);
  lcd.print(F("+==================+"));

  lcd.setCursor(0, 1);
  lcd.print(F("|  !NUEVO RECORD!  |"));

  actualizarPantallaNombre();

  lcd.setCursor(0, 3);
  lcd.print(F("FLECHAS|START GUARDA"));
}

void actualizarPantallaNombre() {
  lcd.setCursor(0, 2);
  lcd.print(F("|    NOMBRE: "));
  lcd.print(nombreNuevo);
  lcd.print(F("   |"));

  lcd.setCursor(COLUMNA_NOMBRE_NUEVO + indiceNombre, 2);
}

void manejarNuevaMarca() {
  if (botonPresionado(BOTON_ARRIBA, ultimoArriba)) {
    nombreNuevo[indiceNombre]++;

    if (nombreNuevo[indiceNombre] > 'Z') {
      nombreNuevo[indiceNombre] = 'A';
    }

    actualizarPantallaNombre();
  }

  if (botonPresionado(BOTON_ABAJO, ultimoAbajo)) {
    nombreNuevo[indiceNombre]--;

    if (nombreNuevo[indiceNombre] < 'A') {
      nombreNuevo[indiceNombre] = 'Z';
    }

    actualizarPantallaNombre();
  }

  if (botonPresionado(BOTON_IZQUIERDA, ultimoIzquierda)) {
    if (indiceNombre == 0) indiceNombre = LETRAS_NOMBRE - 1;
    else indiceNombre--;

    actualizarPantallaNombre();
  }

  if (botonPresionado(BOTON_DERECHA, ultimoDerecha)) {
    indiceNombre++;
    if (indiceNombre >= LETRAS_NOMBRE) indiceNombre = 0;

    actualizarPantallaNombre();
  }

  if (botonPresionado(BOTON_DISPARO_1, ultimoDisparo1)) {
    lcd.noBlink();
    guardarNuevoMarcador();
    mostrarMarcadores();
  }
}

void dibujarPantallaJuego() {
  lcd.clear();
  actualizarBarraEstado(true);
  redibujarAreaJuego();
  areaJuegoPendiente = false;
}

void actualizarBarraEstado(bool forzar) {
  if (juegoPausado) {
    return;
  }

  if (!forzar && millis() - ultimaActualizacionBarraMs < 250) {
    return;
  }

  ultimaActualizacionBarraMs = millis();

  lcd.setCursor(0, 0);

  lcd.print(F("P:"));
  imprimirPuntajeTresDigitos(puntaje);

  lcd.print(F(" V:"));
  lcd.print(vidas);

  lcd.print(F(" N:"));
  lcd.print(nivel);

  lcd.print(F(" SP:"));

  if (modoAdmin) {
    lcd.print(F("AD"));
  }
  else if (especialDisponible()) {
    lcd.print(F("OK"));
  }
  else {
    unsigned long restante = recargaEspecialMs - (millis() - ultimoEspecialMs);
    uint8_t segundos = (restante + 999) / 1000;

    if (segundos < 10) {
      lcd.print(F("0"));
    }

    lcd.print(segundos);
  }

  lcd.print(F(" "));
}

void redibujarAreaJuego() {
  if (pantallaActual != PANTALLA_JUEGO) {
    return;
  }

  uint8_t filaCuadro[COLUMNAS_LCD];

  for (uint8_t filaLCD = FILA_JUEGO_MIN; filaLCD <= FILA_JUEGO_MAX; filaLCD++) {
    construirFilaCuadro(filaLCD, filaCuadro);
    actualizarFilaPorBloques(filaLCD, filaCuadro);
  }
}

void construirFilaCuadro(uint8_t filaLCD, uint8_t filaCuadro[COLUMNAS_LCD]) {
  for (uint8_t col = 0; col < COLUMNAS_LCD; col++) {
    filaCuadro[col] = ' ';
  }

  for (uint8_t i = 0; i < MAXIMO_ENEMIGOS; i++) {
    if (enemigoActivo[i]) {
      if (enemigoY[i] == filaLCD && enemigoX[i] >= COLUMNA_MIN && enemigoX[i] <= COLUMNA_MAX) {
        filaCuadro[enemigoX[i]] = obtenerCaracterEnemigo(enemigoTipo[i]);
      }
    }
  }

  for (uint8_t fila = 0; fila < TOTAL_BALAS; fila++) {
    if (balaActiva[fila]) {
      if (balaY[fila] == filaLCD && balaX[fila] >= COLUMNA_MIN && balaX[fila] <= COLUMNA_MAX) {
        filaCuadro[balaX[fila]] = CAR_BALA;
      }
    }
  }

  if (proyectilEspecialActivo) {
    if (especialY == filaLCD && especialX >= COLUMNA_MIN && especialX <= COLUMNA_MAX) {
      filaCuadro[especialX] = CAR_ESPECIAL;
    }
  }

  if (explosionActiva) {
    insertarExplosionEnFila(filaLCD, filaCuadro);
  }

  if (jugadorY == filaLCD && jugadorX <= COLUMNA_MAX) {
    filaCuadro[jugadorX] = CAR_JUGADOR;
  }
}

uint8_t obtenerCaracterEnemigo(uint8_t tipo) {
  if (tipo == 0) return CAR_ENEMIGO_1;
  if (tipo == 1) return CAR_ENEMIGO_2;
  return CAR_ENEMIGO_3;
}

void insertarExplosionEnFila(uint8_t filaLCD, uint8_t filaCuadro[COLUMNAS_LCD]) {
  for (int8_t dy = -1; dy <= 1; dy++) {
    int8_t y = explosionY + dy;

    if (y != filaLCD) {
      continue;
    }

    if (y < FILA_JUEGO_MIN || y > FILA_JUEGO_MAX) {
      continue;
    }

    for (int8_t dx = -1; dx <= 1; dx++) {
      int8_t x = explosionX + dx;

      if (x < COLUMNA_MIN || x > COLUMNA_MAX) {
        continue;
      }

      if (mascaraExplosion[dy + 1][dx + 1]) {
        filaCuadro[x] = CAR_EXPLOSION;
      }
    }
  }
}

void actualizarFilaPorBloques(uint8_t filaLCD, uint8_t filaCuadro[COLUMNAS_LCD]) {
  uint8_t filaCache = filaLCD - FILA_JUEGO_MIN;
  uint8_t col = 0;

  while (col < COLUMNAS_LCD) {
    if (cacheJuego[filaCache][col] == filaCuadro[col]) {
      col++;
      continue;
    }

    uint8_t inicio = col;

    while (col < COLUMNAS_LCD && cacheJuego[filaCache][col] != filaCuadro[col]) {
      col++;
    }

    lcd.setCursor(inicio, filaLCD);

    for (uint8_t i = inicio; i < col; i++) {
      lcd.write((uint8_t)filaCuadro[i]);
      cacheJuego[filaCache][i] = filaCuadro[i];
    }
  }
}

void mostrarFinPartida() {
  pantallaActual = PANTALLA_FIN_PARTIDA;
  areaJuegoPendiente = false;
  invalidarCacheJuego();

  lcd.noBlink();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(F("+==================+"));

  lcd.setCursor(0, 1);
  lcd.print(F("|  FIN DE PARTIDA  |"));

  lcd.setCursor(0, 2);
  lcd.print(F("|  PUNTAJE: "));
  imprimirPuntajeTresDigitos(puntaje);
  lcd.print(F("    |"));

  lcd.setCursor(0, 3);
  lcd.print(F("|   START = MENU   |"));
}

void manejarFinPartida() {
  if (botonPresionado(BOTON_DISPARO_1, ultimoDisparo1) || botonPresionado(BOTON_DISPARO_2, ultimoDisparo2)) {
    mostrarMenu();
  }
}

void mostrarPausa() {
  areaJuegoPendiente = false;
  invalidarCacheJuego();

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(F("+==================+"));

  lcd.setCursor(0, 1);
  lcd.print(F("|      PAUSA       |"));

  lcd.setCursor(0, 2);
  lcd.print(F("| USART: RESUME    |"));

  lcd.setCursor(0, 3);
  lcd.print(F("+==================+"));
}
