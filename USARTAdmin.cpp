#include "USARTAdmin.h"
#include "Disparos.h"
#include "Enemigos.h"
#include "Marcadores.h"
#include "Pantallas.h"
#include "Puntaje.h"
#include "Sonido.h"

void leerUSARTAdmin() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      if (indiceComando > 0) {
        comandoSerial[indiceComando] = '\0';
        procesarComando(comandoSerial);
        indiceComando = 0;
        comandoSerial[0] = '\0';
      }
    }
    else {
      if (indiceComando < TAMANIO_BUFFER_COMANDO - 1) {
        comandoSerial[indiceComando++] = toupper(c);
        comandoSerial[indiceComando] = '\0';
      }
      else {
        indiceComando = 0;
        comandoSerial[0] = '\0';
        Serial.println(F("ERROR: comando demasiado largo"));
      }
    }
  }
}

void procesarComando(char *comando) {
  while (*comando == ' ') {
    comando++;
  }

  uint8_t longitud = strlen(comando);

  while (longitud > 0 && comando[longitud - 1] == ' ') {
    comando[longitud - 1] = '\0';
    longitud--;
  }

  if (strcmp(comando, "HELP") == 0) {
    mostrarAyudaUSART();
  }
  else if (strcmp(comando, "STATUS") == 0) {
    imprimirEstadoUSART();
  }
  else if (strcmp(comando, "HIGHSCORES") == 0) {
    imprimirMarcadoresUSART();
  }
  else if (strcmp(comando, "RESET_HS") == 0) {
    resetearMarcadores();
    Serial.println(F("OK: marcadores reiniciados"));
  }
  else if (strcmp(comando, "MUSIC ON") == 0) {
    musicaActiva = true;
    Serial.println(F("OK: musica activada"));

    if (pantallaActual == PANTALLA_MENU) {
      mostrarMenu();
    }
  }
  else if (strcmp(comando, "MUSIC OFF") == 0) {
    musicaActiva = false;
    detenerMusica();
    Serial.println(F("OK: musica desactivada"));

    if (pantallaActual == PANTALLA_MENU) {
      mostrarMenu();
    }
  }
  else if (strcmp(comando, "PAUSE") == 0) {
    if (pantallaActual == PANTALLA_JUEGO) {
      juegoPausado = true;
      areaJuegoPendiente = false;
      mostrarPausa();
      Serial.println(F("OK: juego pausado"));
    } else {
      Serial.println(F("ERROR: solo se puede pausar durante el juego"));
    }
  }
  else if (strcmp(comando, "RESUME") == 0) {
    if (pantallaActual == PANTALLA_JUEGO) {
      juegoPausado = false;
      invalidarCacheJuego();
      redibujarAreaJuego();
      actualizarBarraEstado(true);
      Serial.println(F("OK: juego reanudado"));
    } else {
      Serial.println(F("ERROR: no hay partida activa"));
    }
  }
  else if (strncmp(comando, "LIVES ", 6) == 0) {
    int nuevasVidas = atoi(comando + 6);

    if (nuevasVidas >= 0 && nuevasVidas <= 9) {
      vidas = nuevasVidas;

      if (pantallaActual == PANTALLA_JUEGO) {
        actualizarBarraEstado(true);
      }

      Serial.print(F("OK: vidas = "));
      Serial.println(vidas);

      if (vidas == 0 && pantallaActual == PANTALLA_JUEGO && !modoAdmin) {
        finalizarPartida();
      }
    } else {
      Serial.println(F("ERROR: usa LIVES 0-9"));
    }
  }
  else if (strncmp(comando, "SCORE ", 6) == 0) {
    int nuevoPuntaje = atoi(comando + 6);

    if (nuevoPuntaje >= 0 && nuevoPuntaje <= PUNTAJE_MAXIMO) {
      puntaje = nuevoPuntaje;
      actualizarDificultad();

      if (pantallaActual == PANTALLA_JUEGO) {
        actualizarBarraEstado(true);
      }

      Serial.print(F("OK: puntaje = "));
      Serial.println(puntaje);
      Serial.print(F("OK: nivel = "));
      Serial.println(nivel);
    } else {
      Serial.println(F("ERROR: usa SCORE 0-999"));
    }
  }
  else if (strcmp(comando, "ADMIN ON") == 0) {
    modoAdmin = true;

    if (pantallaActual == PANTALLA_JUEGO) {
      actualizarBarraEstado(true);
    }

    Serial.println(F("OK: MODO ADMIN ON"));
    Serial.println(F("Inmortalidad activada. Especial sin cooldown."));
  }
  else if (strcmp(comando, "ADMIN OFF") == 0) {
    modoAdmin = false;

    if (pantallaActual == PANTALLA_JUEGO) {
      actualizarBarraEstado(true);

      if (vidas == 0) {
        finalizarPartida();
      }
    }

    Serial.println(F("OK: MODO ADMIN OFF"));
  }
  else {
    Serial.println(F("ERROR: comando no reconocido. Escribe HELP"));
  }
}

void mostrarAyudaUSART() {
  Serial.println(F("=== MODO ADMIN USART ==="));
  Serial.println(F("HELP       - lista comandos"));
  Serial.println(F("STATUS     - estado del juego"));
  Serial.println(F("HIGHSCORES - muestra top 3"));
  Serial.println(F("RESET_HS   - reinicia marcadores"));
  Serial.println(F("MUSIC ON   - activa musica"));
  Serial.println(F("MUSIC OFF  - desactiva musica"));
  Serial.println(F("PAUSE      - pausa juego"));
  Serial.println(F("RESUME     - reanuda juego"));
  Serial.println(F("LIVES n    - cambia vidas 0-9"));
  Serial.println(F("SCORE n    - cambia puntaje 0-999"));
  Serial.println(F("ADMIN ON   - inmortal + especial sin cooldown"));
  Serial.println(F("ADMIN OFF  - desactiva modo admin"));
}

void imprimirEstadoUSART() {
  Serial.println(F("=== STATUS ==="));

  Serial.print(F("Pantalla: "));

  if (pantallaActual == PANTALLA_INICIO) Serial.println(F("INICIO"));
  else if (pantallaActual == PANTALLA_MENU) Serial.println(F("MENU"));
  else if (pantallaActual == PANTALLA_MARCADORES) Serial.println(F("MARCADORES"));
  else if (pantallaActual == PANTALLA_JUEGO) Serial.println(F("JUEGO"));
  else if (pantallaActual == PANTALLA_NUEVA_MARCA) Serial.println(F("NUEVA MARCA"));
  else if (pantallaActual == PANTALLA_FIN_PARTIDA) Serial.println(F("FIN PARTIDA"));

  Serial.print(F("Puntaje: "));
  Serial.println(puntaje);

  Serial.print(F("Vidas: "));
  Serial.println(vidas);

  Serial.print(F("Nivel: "));
  Serial.println(nivel);

  Serial.print(F("Enemigos activos: "));
  Serial.println(contarEnemigosActivos());

  Serial.print(F("Musica: "));
  if (musicaActiva) Serial.println(F("ON"));
  else Serial.println(F("OFF"));

  Serial.print(F("Pausa: "));
  if (juegoPausado) Serial.println(F("SI"));
  else Serial.println(F("NO"));

  Serial.print(F("Modo admin: "));
  if (modoAdmin) Serial.println(F("ON"));
  else Serial.println(F("OFF"));

  Serial.print(F("Especial: "));
  if (especialDisponible()) Serial.println(F("DISPONIBLE"));
  else Serial.println(F("COOLDOWN"));
}

void imprimirMarcadoresUSART() {
  Serial.println(F("=== MARCADORES ==="));

  for (uint8_t i = 0; i < TOTAL_MARCADORES; i++) {
    Serial.print(i + 1);
    Serial.print(F(". "));
    Serial.print(marcadores[i].nombre);
    Serial.print(F(" "));

    if (marcadores[i].puntos < 100) Serial.print(F("0"));
    if (marcadores[i].puntos < 10) Serial.print(F("0"));
    Serial.println(marcadores[i].puntos);
  }
}
