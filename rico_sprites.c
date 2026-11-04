#include "rico_sprites.h"

ALLEGRO_BITMAP *carregar_atlas_rico(void) {
    return al_load_bitmap("assets/rico_sprites.png");
}

ALLEGRO_BITMAP *recortar_rico(ALLEGRO_BITMAP *atlas, int pose) {
    int x;
    int y;
    int largura;
    int altura;

    if (atlas == NULL) {
        return NULL;
    }

    largura = 256;
    altura = 384;

    if (pose == RICO_PARADO) {
        x = 0;
        y = 0;
    } else if (pose == RICO_ANDANDO) {
        x = 256;
        y = 0;
    } else if (pose == RICO_PULANDO) {
        x = 512;
        y = 0;
    } else if (pose == RICO_CORRIDA) {
        x = 0;
        y = 384;
        largura = 543;
        altura = 181;
    } else {
        return NULL;
    }

    return al_create_sub_bitmap(atlas, x, y, largura, altura);
}
