[Español](README_es.md) | [English](README.md)

# Motor de Rasterización 3D por Software



https://github.com/user-attachments/assets/a24dbd97-5822-4916-aaac-b31f93a6006e



Un motor gráfico 3D construido completamente desde cero en C puro. Este proyecto implementa una pipeline de rasterización por software completa, prescindiendo de APIs gráficas modernas (como OpenGL o Vulkan) para calcular la proyección, iluminación y rasterización de píxeles a nivel matemático utilizando únicamente la CPU.

La salida por pantalla, la gestión de la ventana y el manejo de eventos de entrada se gestionan a través de SDL2.

## Características Técnicas

*   **Rasterización por Software:** Implementación de algoritmos de trazado de líneas (Bresenham, DDA) y rellenado de polígonos.
*   **Texturizado con Corrección de Perspectiva:** Mapeo UV que utiliza la inversa de la profundidad (1/Z) e interpolación en el rasterizador DDA para evitar distorsiones afines.
*   **Gestión de Profundidad:** Implementación de un Z-Buffer por píxel para garantizar la correcta oclusión de la geometría en la escena.
*   **Iluminación Base:** Sistema de *Flat Shading* con cálculo de luz ambiental (factor 0.2) basado en el producto escalar de las normales de las caras.
*   **Cámara Libre (FPS):** Sistema de cámara voladora calculada mediante trigonometría (Pitch/Yaw), controlable con ratón y teclado (WASD).
*   **Parser de Modelos 3D:** Cargador personalizado para archivos `.obj` triangulados con gestión de memoria dinámica y prevención de fugas mediante `freeMesh`.
*   **Carga de Texturas:** Integración de la librería externa de terceros `stb_image` para el parseo y volcado de texturas en memoria.

## Arquitectura del Proyecto

El código está estructurado de manera modular para separar la lógica matemática, las transformaciones espaciales y el estado global de la aplicación:

*   `main.c` - Director de orquesta: gestiona el bucle principal, el estado del motor (`EngineState`), el control de *deltatime* y las llamadas de la pipeline.
*   `display.c` - Inicialización de SDL2, manipulación del frame buffer, Z-buffer, procesamiento de triángulos y pintado final de píxeles.
*   `camera.c` - Lógica de vista, vectores de dirección y rotaciones de cámara en el espacio 3D.
*   `mesh.c` - Carga dinámica de vértices, coordenadas UV, parser de archivos `.obj` y liberación segura de recursos en cierre (`freeMesh`).
*   `triangle.c` - Algoritmos de recorte espacial (*clipping*), cálculo de normales y eliminación de caras ocultas (*backface culling*).
*   `transform.c` - Lógica matemática de rotaciones trigonométricas (ejes X, Y, Z) y transformaciones espaciales del modelo.
*   `vector.c` - Librería matemática propia optimizada para operaciones de vectores 2D, 3D y 4D (productos escalar y cruzado, normalización).

## Compilación y Ejecución

### Dependencias
*   Compilador C (GCC / MinGW / Clang)
*   [SDL2](https://www.libsdl.org/) (Gestión de ventana, contexto gráfico e input)
*   `stb_image.h` (Librería externa de dominio público para carga de imágenes)

### Entornos Soportados
El proyecto está preparado para compilarse en entornos Windows nativos usando MSYS2 (MinGW) o a través de WSL (Ubuntu / WSLg).

## Controles

*   **W / A / S / D:** Movimiento de la cámara (Adelante, Izquierda, Atrás, Derecha).
*   **Ratón:** Rotación de la vista (Pitch y Yaw mediante modo relativo).
*   **ESC:** Cerrar el motor y liberar la memoria de forma segura.

```bash
# Ejemplo de comando de compilación:
gcc src/*.c -lmingw32 -lSDL2main -lSDL2 -lm -o renderer3d
