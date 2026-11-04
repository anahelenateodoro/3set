CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic
ALLEGRO = allegro_main-5 allegro-5 allegro_primitives-5 allegro_image-5 allegro_font-5

all: duofit

duofit: main.c atletismo.c ana_sprites.c rico_sprites.c ana_sprites.h rico_sprites.h
	$(CC) $(CFLAGS) $(shell pkg-config --cflags $(ALLEGRO)) main.c atletismo.c ana_sprites.c rico_sprites.c -o $@ $(shell pkg-config --libs $(ALLEGRO))

run: duofit
	./duofit

clean:
	rm -f duofit

.PHONY: all run clean
