#ifndef ANA_SPRITES_H
#define ANA_SPRITES_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>


#define ANA_PARADA 0
#define ANA_ANDANDO1 1
#define ANA_ANDANDO2 2
#define ANA_CORRENDO2 3
#define ANA_PULANDO 4
ALLEGRO_BITMAP *carregar_atlas_ana(void);
ALLEGRO_BITMAP *recortar_ana(ALLEGRO_BITMAP *atlas, int pose);
#endif
