#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

void jogar_atletismo(void);

int main(void) {
    if (al_init() == false) {
        printf("Erro ao iniciar Allegro.\n");
        return 1;
    }

    if (al_install_keyboard() == false) {
        printf("Erro ao iniciar teclado.\n");
        return 1;
    }

    if (al_init_primitives_addon() == false) {
        printf("Erro ao iniciar desenhos.\n");
        return 1;
    }

    if (al_init_image_addon() == false) {
        printf("Erro ao iniciar imagens.\n");
        return 1;
    }

    al_init_font_addon();
    jogar_atletismo();

    return 0;
}
