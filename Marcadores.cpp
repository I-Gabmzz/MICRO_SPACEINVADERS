#include "Marcadores.h"
#include "Pantallas.h"

void cargarMarcadores() {
  byte firma = EEPROM.read(EEPROM_DIRECCION_FIRMA);

  if (firma != EEPROM_FIRMA) {
    inicializarMarcadores();
    guardarMarcadores();
    return;
  }

  int direccion = EEPROM_DIRECCION_DATOS;

  for (uint8_t i = 0; i < TOTAL_MARCADORES; i++) {
    EEPROM.get(direccion, marcadores[i]);
    direccion += sizeof(Marcador);

    marcadores[i].nombre[LETRAS_NOMBRE] = '\0';

    if (marcadores[i].puntos > PUNTAJE_MAXIMO) {
      inicializarMarcadores();
      guardarMarcadores();
      return;
    }
  }
}

void inicializarMarcadores() {
  strcpy(marcadores[0].nombre, "AAA");
  marcadores[0].puntos = 0;

  strcpy(marcadores[1].nombre, "BBB");
  marcadores[1].puntos = 0;

  strcpy(marcadores[2].nombre, "CCC");
  marcadores[2].puntos = 0;
}

void guardarMarcadores() {
  EEPROM.update(EEPROM_DIRECCION_FIRMA, EEPROM_FIRMA);

  int direccion = EEPROM_DIRECCION_DATOS;

  for (uint8_t i = 0; i < TOTAL_MARCADORES; i++) {
    EEPROM.put(direccion, marcadores[i]);
    direccion += sizeof(Marcador);
  }
}

int8_t obtenerPosicionMarcador(uint16_t puntos) {
  for (uint8_t i = 0; i < TOTAL_MARCADORES; i++) {
    if (puntos > marcadores[i].puntos) {
      return i;
    }
  }

  return -1;
}

void guardarNuevoMarcador() {
  if (posicionNuevoMarcador < 0 || posicionNuevoMarcador >= TOTAL_MARCADORES) {
    return;
  }

  Marcador nuevo;
  strcpy(nuevo.nombre, nombreNuevo);
  nuevo.puntos = puntaje;

  for (int8_t i = TOTAL_MARCADORES - 1; i > posicionNuevoMarcador; i--) {
    marcadores[i] = marcadores[i - 1];
  }

  marcadores[posicionNuevoMarcador] = nuevo;
  guardarMarcadores();
}

void resetearMarcadores() {
  inicializarMarcadores();
  guardarMarcadores();

  if (pantallaActual == PANTALLA_MARCADORES) {
    mostrarMarcadores();
  }
}
