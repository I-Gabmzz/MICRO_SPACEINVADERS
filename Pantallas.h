#ifndef PANTALLAS_H
#define PANTALLAS_H

#include "EstadoJuego.h"

void limpiarFila(uint8_t fila);
void imprimirPuntajeTresDigitos(uint16_t valor);
void invalidarCacheJuego();
void solicitarRedibujoJuego();
void procesarRedibujoPendiente();
void mostrarInicio();
void manejarInicio();
void mostrarMenu();
void imprimirOpcionMenu(uint8_t fila, const __FlashStringHelper *texto, bool seleccionada);
void manejarMenu();
void mostrarMarcadores();
void manejarMarcadores();
void mostrarNuevaMarca();
void actualizarPantallaNombre();
void manejarNuevaMarca();
void dibujarPantallaJuego();
void actualizarBarraEstado(bool forzar);
void redibujarAreaJuego();
void construirFilaCuadro(uint8_t filaLCD, uint8_t filaCuadro[]);
uint8_t obtenerCaracterEnemigo(uint8_t tipo);
void insertarExplosionEnFila(uint8_t filaLCD, uint8_t filaCuadro[]);
void actualizarFilaPorBloques(uint8_t filaLCD, uint8_t filaCuadro[]);
void mostrarFinPartida();
void manejarFinPartida();
void mostrarPausa();

#endif
