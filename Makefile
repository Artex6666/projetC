# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Iinclude -Wall -Wextra -g -DSDL_MAIN_HANDLED


# Options de liaison (SDL2, SDL2_ttf, SDL2_mixer, SDL2_gfx, SDL2_image)
LDFLAGS = -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx -lSDL2_image


# Répertoires
SRC_DIR = src
OBJ_DIR = build
INCLUDE_DIR = include
LIB_DIR = lib
ASSETS_DIR = assets

# Fichiers source et objets
SRCS_BACK = $(wildcard $(SRC_DIR)/back/*.c)
SRCS_FRONT = $(wildcard $(SRC_DIR)/front/*.c)
SRCS_MAIN = $(wildcard $(SRC_DIR)/*.c)

OBJS_BACK = $(SRCS_BACK:$(SRC_DIR)/back/%.c=$(OBJ_DIR)/back/%.o)
OBJS_FRONT = $(SRCS_FRONT:$(SRC_DIR)/front/%.c=$(OBJ_DIR)/front/%.o)
OBJS_MAIN = $(SRCS_MAIN:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

OBJS = $(OBJS_BACK) $(OBJS_FRONT) $(OBJS_MAIN)

# Nom de l'exécutable
TARGET = motus

# Règle par défaut
all: $(TARGET)

# Compilation de l'exécutable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers objets pour le back-end
$(OBJ_DIR)/back/%.o: $(SRC_DIR)/back/%.c
	mkdir -p $(OBJ_DIR)/back
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation des fichiers objets pour le front-end
$(OBJ_DIR)/front/%.o: $(SRC_DIR)/front/%.c
	mkdir -p $(OBJ_DIR)/front
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation des fichiers objets pour le main
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Nettoyage complet (y compris les fichiers de sortie dans assets/output)
mrproper: clean
	rm -rf $(ASSETS_DIR)/output/

.PHONY: all clean mrproper