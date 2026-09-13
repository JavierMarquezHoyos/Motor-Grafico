[Español](README_es.md) | [English](README.md)

# 3D Software Rasterizer Engine



<video src="https://github.com/user-attachments/assets/d7ccf569-a5f2-4eb9-8ad5-a7103efe40bf" autoplay loop muted playsinline width="100%"></video>



A 3D graphics engine built entirely from scratch in pure C. This project implements a complete software rasterization pipeline, bypassing modern graphics APIs (like OpenGL or Vulkan) to calculate projection, lighting, and pixel rasterization at a mathematical level using only the CPU.

Screen output, window management, and input event handling are managed through SDL2.

## Technical Features

*   **Software Rasterization:** Implementation of line drawing algorithms (Bresenham, DDA) and polygon filling.
*   **Perspective-Correct Texturing:** UV mapping utilizing depth inversion (1/Z) and DDA rasterizer interpolation to prevent affine texture distortion.
*   **Depth Management:** Per-pixel Z-Buffer implementation to ensure proper geometry occlusion in the scene.
*   **Base Lighting:** Flat Shading system with ambient light calculation (0.2 factor) based on the dot product of face normals.
*   **Free Camera (FPS):** Fly-through camera system calculated via trigonometry (Pitch/Yaw), controllable with mouse and keyboard (WASD).
*   **3D Model Parser:** Custom loader for triangulated `.obj` files with dynamic memory management and leak prevention using `freeMesh`.
*   **Texture Loading:** Integration of the third-party external library `stb_image` for parsing and loading textures into memory.

## Project Architecture

The code is structured modularly to separate mathematical logic, spatial transformations, and the global application state:

*   `main.c` - Orchestrator: manages the main loop, engine state (`EngineState`), delta time control, and pipeline calls.
*   `display.c` - SDL2 initialization, frame buffer manipulation, Z-buffer, triangle processing, and final pixel drawing.
*   `camera.c` - View logic, direction vectors, and 3D space camera rotations.
*   `mesh.c` - Dynamic loading of vertices, UV coordinates, `.obj` file parser, and safe resource deallocation on close (`freeMesh`).
*   `triangle.c` - Spatial clipping algorithms, normal calculations, and backface culling.
*   `transform.c` - Trigonometric rotation logic (X, Y, Z axes) and model spatial transformations.
*   `vector.c` - Custom optimized math library for 2D, 3D, and 4D vector operations (dot and cross products, normalization).

## Build and Run

### Dependencies
*   C Compiler (GCC / MinGW / Clang)
*   [SDL2](https://www.libsdl.org/) (Window management, graphics context, and input)
*   `stb_image.h` (Public domain third-party library for image loading)

### Supported Environments
The project is set up to compile natively on Windows using MSYS2 (MinGW) or through WSL (Ubuntu / WSLg).

## Controls

*   **W / A / S / D:** Camera movement (Forward, Left, Backward, Right).
*   **Mouse:** View rotation (Pitch and Yaw via relative mode).
*   **ESC:** Close the engine and safely free memory.

```bash
# Compilation command example:
gcc src/*.c -lmingw32 -lSDL2main -lSDL2 -lm -o renderer3d
