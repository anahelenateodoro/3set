#ifndef RICO_SPRITES_H
#define RICO_SPRITES_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define RICO_PARADO 0
#define RICO_ANDANDO 1
#define RICO_PULANDO 2
#define RICO_CORRIDA 3

ALLEGRO_BITMAP *carregar_atlas_rico(void);
ALLEGRO_BITMAP *recortar_rico(ALLEGRO_BITMAP *atlas, int pose);

#endif
