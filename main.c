#include "duofit.h"
#include <stdlib.h> 
#include <time.h>     

int main(int argc, char** argv) {

    if (!al_init()) {
        printf("Erro ao iniciar o Allegro.\n");
        return 1;
    }
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    srand(time(NULL));   // para o rand da goleira mudar a cada vez

    // o jogo e 2D, entao pedi para a janela nao guardar memoria de profundidade (usada em 3D)
    al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 0, ALLEGRO_SUGGEST);

    // uma janela so para o jogo todo (o menu e as fases desenham nela)
    ALLEGRO_DISPLAY* janela = al_create_display(LARGURA, ALTURA);
    if (janela == NULL) {
        printf("Erro ao criar a janela.\n");
        return 1;
    }
    al_set_window_title(janela, "DuoFit");

    // No Visual Studio as vezes o programa roda em outra pasta.
    // Se nao achar a pasta assets aqui, sobe uma pasta e tenta de novo (ate 3 vezes).
    int subidas = 0;
    while (!al_filename_exists("assets/bola.png") && subidas < 3) {
        al_change_directory("..");
        subidas++;
    }

    if (!carregar_recursos()) {
        printf("Erro: nao achei as imagens. Coloque a pasta assets junto do projeto.\n");
        liberar_recursos();
        al_destroy_display(janela);
        return 1;
    }

    // MVP: so o menu e a fase de handebol
    bool rodando = true;
    while (rodando) {
        int escolha = mostrar_menu();

        if (escolha == 1) {
            int resultado = jogar_handebol();
            if (resultado == FECHAR_JOGO) {
                rodando = false;   // clicou no X da janela
            }
            // se nao, volta para o menu
        }
        else {
            rodando = false;   // escolheu SAIR
        }
    }

    liberar_recursos();
    al_destroy_display(janela);
    return 0;
}
