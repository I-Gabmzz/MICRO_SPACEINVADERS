#include "AVRCompat.h"
#include "Juego.h"

int main(void) {
  sistemaInicializar();

  inicializarJuego();

  while (true) {
    cicloJuego();
  }

  return 0;
}
