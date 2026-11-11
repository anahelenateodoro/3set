
#include "duofit.h"

int mostrar_menu(void) {

    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(al_get_current_display()));
    al_register_event_source(fila, al_get_keyboard_event_source());

    char* botoes[2] = { "JOGAR HANDEBOL", "SAIR" };
    int opcao = 0;      // botao marcado (0 = jogar, 1 = sair)
    int escolha = -1;   // -1 = ainda nao escolheu

    while (escolha == -1) {

        // ---------- desenhar ----------
        al_clear_to_color(al_map_rgb(20, 45, 100));
        escrever(fonteGrande, al_map_rgb(255, 140, 0), 110, "DuoFit");
        escrever(fonte, al_map_rgb(255, 255, 255), 190, "Jogo educativo de Educacao Fisica - versao de demonstracao");

        for (int i = 0; i < 2; i++) {
            ALLEGRO_COLOR cor = al_map_rgb(40, 90, 170);
            if (opcao == i) {
                cor = al_map_rgb(255, 140, 0);   // botao marcado fica laranja
            }
            // cada botao fica 90 pixels abaixo do outro
            al_draw_filled_rounded_rectangle(400, 280 + i * 90, 800, 345 + i * 90, 12, 12, cor);
            escrever(fonteMenu, al_map_rgb(255, 255, 255), 290 + i * 90, botoes[i]);
        }
        escrever(fonteMenu, al_map_rgb(255, 255, 255), 700, "W/S para escolher | ENTER para entrar");
        al_flip_display();

        // ---------- esperar o jogador ----------
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            escolha = 0;
        }

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            int tecla = evento.keyboard.keycode;
            if (tecla == ALLEGRO_KEY_ESCAPE) {
                escolha = 0;
            }
            if ((tecla == ALLEGRO_KEY_W || tecla == ALLEGRO_KEY_UP) && opcao > 0) {
                opcao--;
            }
            if ((tecla == ALLEGRO_KEY_S || tecla == ALLEGRO_KEY_DOWN) && opcao < 1) {
                opcao++;
            }
            if (tecla == ALLEGRO_KEY_ENTER) {
                if (opcao == 0) {
                    escolha = 1;   // jogar
                }
                else {
                    escolha = 0;   // sair
                }
            }
        }
    }

    al_destroy_event_queue(fila);
    return escolha;
}
