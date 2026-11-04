#include "ana_sprites.h"

ALLEGRO_BITMAP *carregar_atlas_ana(void) {
    return al_load_bitmap("assets/ana_sprites.png");
}

ALLEGRO_BITMAP *recortar_ana(ALLEGRO_BITMAP *atlas, int pose) {
    int x;
    int y;

    if (atlas == NULL) {
        return NULL;
    }

    if (pose == ANA_PARADA) {
        x = 0;
        y = 0;
    } else if (pose == ANA_ANDANDO1) {
        x = 256;
        y = 0;
    } else if (pose == ANA_ANDANDO2) {
        x = 512;
        y = 0;
    } else if (pose == ANA_CORRENDO2) {
        x = 0;
        y = 384;
    } else if (pose == ANA_PULANDO) {
        x = 256;
        y = 384;
    } else {
        return NULL;
    }

    return al_create_sub_bitmap(atlas, x, y, 256, 384);
}