# MICRO_SPACEINVADERS

Proyecto final de la materia de **Microcontroladores**, desarrollado como una versión tipo **Space Invaders** para un microcontrolador AVR/ATmega328P, utilizando una pantalla **LCD 20x4 con módulo I2C**, botones físicos para la entrada del jugador y un **buzzer pasivo** para efectos de sonido.

El proyecto está organizado de forma modular en archivos `.cpp` y `.h`, separando la lógica del juego, el manejo de enemigos, disparos, colisiones, pantallas, puntaje, sonido, caracteres personalizados de la LCD, temporización y comunicación USART para modo administrador.

## Características principales

- Juego estilo Space Invaders implementado en C/C++ para AVR.
- Visualización en LCD 20x4 mediante comunicación I2C.
- Uso de caracteres personalizados para representar jugador, enemigos, disparos y elementos gráficos.
- Control mediante botones físicos.
- Efectos de sonido con buzzer pasivo.
- Sistema de puntaje y marcadores.
- Organización por módulos para facilitar lectura, mantenimiento y explicación del código.
- Compatibilidad con Microchip Studio.
- Comunicación USART destinada al modo administrador o depuración necesaria.

## Hardware utilizado

- ATmega328P o placa compatible basada en AVR.
- Pantalla LCD 20x4 con módulo I2C.
- Botones físicos para control del jugador y acciones del juego.
- Buzzer pasivo.
- Resistencias y conexiones básicas para entradas digitales.
- Protoboard o placa perforada para montaje físico.

## Estructura general del proyecto

```text
MICRO_SPACEINVADERS/
├── MICRO_SPACEINVADERS.atsln
├── MICRO_SPACEINVADERS/
│   ├── main.cpp
│   ├── ConfigJuego.h
│   ├── AudioJuegoAVR.cpp / AudioJuegoAVR.h
│   ├── AVRCompat.cpp / AVRCompat.h
│   ├── CaracteresLCD.cpp / CaracteresLCD.h
│   ├── Colisiones.cpp / Colisiones.h
│   ├── Disparos.cpp / Disparos.h
│   ├── Enemigos.cpp / Enemigos.h
│   ├── Entrada.cpp / Entrada.h
│   ├── EstadoJuego.cpp / EstadoJuego.h
│   ├── I2CJuegoAVR.cpp / I2CJuegoAVR.h
│   ├── Juego.cpp / Juego.h
│   ├── Jugador.cpp / Jugador.h
│   ├── Marcadores.cpp / Marcadores.h
│   ├── Pantallas.cpp / Pantallas.h
│   ├── Puntaje.cpp / Puntaje.h
│   ├── Sonido.cpp / Sonido.h
│   ├── TemporizadorJuego.cpp / TemporizadorJuego.h
│   └── USARTAdmin.cpp / USARTAdmin.h
```

## Descripción de módulos

| Archivo / módulo | Función principal |
|---|---|
| `main.cpp` | Punto de entrada del programa. Inicializa y ejecuta el flujo principal. |
| `ConfigJuego.h` | Define constantes, pines, parámetros generales y configuración del juego. |
| `Juego.cpp/.h` | Controla la lógica principal del juego y coordina los demás módulos. |
| `EstadoJuego.cpp/.h` | Maneja los estados del sistema, como inicio, juego activo, pausa o fin de partida. |
| `Jugador.cpp/.h` | Controla la posición y comportamiento del jugador. |
| `Enemigos.cpp/.h` | Maneja la lógica, movimiento y estado de los enemigos. |
| `Disparos.cpp/.h` | Administra los disparos del jugador y su actualización en pantalla. |
| `Colisiones.cpp/.h` | Verifica impactos entre disparos, enemigos y otros elementos del juego. |
| `Pantallas.cpp/.h` | Gestiona las pantallas, mensajes e impresión de elementos en la LCD. |
| `CaracteresLCD.cpp/.h` | Define caracteres personalizados cargados en la memoria CGRAM de la LCD. |
| `Marcadores.cpp/.h` | Muestra y actualiza marcadores visibles durante la partida. |
| `Puntaje.cpp/.h` | Controla el cálculo y almacenamiento del puntaje. |
| `Sonido.cpp/.h` | Ejecuta efectos de sonido y señales auditivas con el buzzer. |
| `AudioJuegoAVR.cpp/.h` | Contiene melodías o secuencias de audio del juego. |
| `I2CJuegoAVR.cpp/.h` | Controla funciones relacionadas con comunicación I2C para la LCD. |
| `USARTAdmin.cpp/.h` | Maneja comunicación serial para funciones administrativas. |
| `AVRCompat.cpp/.h` | Incluye funciones de compatibilidad y apoyo para trabajar con AVR. |

## Compilación

1. Abrir `MICRO_SPACEINVADERS.atsln` en **Microchip Studio**.
2. Verificar que el microcontrolador seleccionado corresponda al usado en el montaje, por ejemplo ATmega328P.
3. Confirmar la frecuencia de trabajo del microcontrolador, normalmente `F_CPU = 16000000UL` si se utiliza reloj de 16 MHz.
4. Compilar el proyecto desde Microchip Studio.
5. El archivo `.hex` se genera en la carpeta de compilación, pero no se recomienda subirlo al repositorio porque es un archivo generado automáticamente.

## Archivos que no se deben subir

Para mantener el repositorio limpio, se excluyen carpetas y archivos generados por Microchip Studio o por la compilación, como:

- `.vs/`
- `Debug/`
- `Release/`
- `*.o`
- `*.d`
- `*.elf`
- `*.hex`
- `*.map`
- `*.lss`
- `*.srec`
- `*.eep`

Estos archivos pueden volver a generarse al compilar el proyecto.

## Estado del proyecto

Proyecto en desarrollo/finalización para práctica o entrega académica de Microcontroladores.

## Autor

Ángel Gabriel Manjarrez Moreno
