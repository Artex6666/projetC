# Compilateur et options
CC = gcc
CFLAGS = -I../include -Wall -Wextra -g -DSDL_MAIN_HANDLED
LDFLAGS = -L../lib -lmingw32 -lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

# Fichier source et cible
SRC = front_test.c
TARGET = front_test.exe

# Règles
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
