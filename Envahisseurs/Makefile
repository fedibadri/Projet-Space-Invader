CC = gcc
CFLAGS = -Wall -Wextra -g -Wno-type-limits -IGraphique-2.0 -IListeChainee
LDFLAGS = $(shell sdl2-config --cflags --libs) -lSDL2_ttf -lm

TARGET = envahisseurs
SRCS = main.c ListeChainee/liste.c jeu.c Graphique-2.0/libgraph.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean














