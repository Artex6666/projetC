# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Iinclude -Wall -Wextra -g -DSDL_MAIN_HANDLED

# Options de liaison (SDL2, SDL2_ttf, SDL2_mixer, SDL2_gfx, SDL2_image, ENet)
LDFLAGS = -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx -lSDL2_image -lenet

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

# Ajout des fichiers spécifiques pour le client et le serveur
CLIENT_SRC = $(SRC_DIR)/client.c
CLIENT_OBJ = $(OBJ_DIR)/client.o
SERVEUR_SRC = $(SRC_DIR)/serveur.c
SERVEUR_OBJ = $(OBJ_DIR)/serveur.o

# Noms des exécutables
TARGET = motus
BIN_CLIENT = client
BIN_SERVEUR = serveur

# Règle par défaut : compile tout
all: $(TARGET) $(BIN_CLIENT) $(BIN_SERVEUR)

# Compilation de l'exécutable principal (jeu)
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compilation du client
$(BIN_CLIENT): $(CLIENT_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compilation du serveur
$(BIN_SERVEUR): $(SERVEUR_OBJ)
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

# Compilation des fichiers objets pour le client et le serveur
$(OBJ_DIR)/client.o: $(SRC_DIR)/client.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/serveur.o: $(SRC_DIR)/serveur.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(BIN_CLIENT) $(BIN_SERVEUR)

# Nettoyage complet (y compris les fichiers de sortie dans assets/output)
mrproper: clean
	rm -rf $(ASSETS_DIR)/output/

.PHONY: all clean mrproper
