# Compilateur et options
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
LDFLAGS = -Llib -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

# Fichiers source
SRC = front_test.c

# Nom de l'exécutable
TARGET = front_test

# Règles
all: $(TARGET)

# Compilation de l'exécutable
$(TARGET): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

# Nettoyage des fichiers générés
clean:
	rm -f $(TARGET)

.PHONY: all clean
