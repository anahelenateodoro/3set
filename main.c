#include "duofit.h"
#include <stdlib.h> 
#include <time.h>     

int main(int argc, char** argv) {

    if (al_init() == false) {
        printf("Erro ao iniciar o Allegro.\n");
        return 1;
    }
    
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    srand(time(NULL));   // para o rand da goleira mudar a cada vez

    al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 0, ALLEGRO_SUGGEST);

    ALLEGRO_DISPLAY* janela = al_create_display(LARGURA, ALTURA);
    if (janela == NULL) {
        printf("Erro ao criar a janela.\n");
        return 1;
    }
    al_set_window_title(janela, "DuoFit");

    // Se nao achar a pasta assets aqui, sobe uma pasta e tenta de novo (ate 3 vezes).
    int subidas = 0;
    while (al_filename_exists("assets/bola.png") == false && subidas < 3) {
        al_change_directory("..");
        subidas++;
    }

    if (carregar_recursos() == false) {
        printf("Erro: nao achei as imagens. Coloque a pasta assets junto do projeto.\n");
        liberar_recursos();
        al_destroy_display(janela);
        return 1;
    }

    bool rodando = true;
    while (rodando) {
        int escolha = mostrar_menu();

        if (escolha == 1) {
            int resultado = jogar_handebol();
            if (resultado == FECHAR_JOGO) {
                rodando = false;   // clicou no X da janela
            }
        }
        else {
            rodando = false;   // escolheu SAIR
        }
    }

    liberar_recursos();
    al_destroy_display(janela);
    return 0;
}
