# Makefile para compilar motor gráfico en C usando SDL2(hecho por IA por pereza)
# El compilador que vamos a usar
CC = gcc

# Banderas de compilación: 
# -Wall y -Wextra muestran todos los avisos/errores
# -std=c99 usa el estándar moderno de C
# -I./include le dice dónde buscar tus archivos .h
CFLAGS = -Wall -Wextra -std=c99 -I./include

# Banderas del Linker: 
# -lSDL2 le dice que pegue la librería gráfica al final
LDFLAGS = -lSDL2

# Busca automáticamente todos tus archivos .c dentro de la carpeta src/
SRC = $(wildcard src/*.c)

# El nombre del programa final que vas a ejecutar
EXEC = motor

# La regla principal (lo que pasa cuando escribes 'make' en la consola)
build:
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC) $(LDFLAGS)

# Una regla extra para limpiar tu entorno
clean:
	rm -f $(EXEC)