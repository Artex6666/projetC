# Compilateur et options
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
LDFLAGS = -Llib -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

# Répertoires
SRC_DIR = src
OBJ_DIR = build
INCLUDE_DIR = include
LIB_DIR = lib
ASSETS_DIR = assets

# Fichiers source et objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Nom de l'exécutable
TARGET = motus

# Règles
all: $(TARGET)

# Compilation de l'exécutable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Nettoyage complet (y compris assets générés, si nécessaire)
mrproper: clean
	rm -rf $(ASSETS_DIR)/output/

.PHONY: all clean mrproper
