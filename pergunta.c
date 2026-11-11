

#include "duofit.h"

int fazer_pergunta(char* esporte, char* pergunta, char* opcoes[3], int correta, char* explicacao) {

    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();
    al_register_event_source(fila, al_get_display_event_source(al_get_current_display()));
    al_register_event_source(fila, al_get_keyboard_event_source());

    // mesmas cores das telas do handebol
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR amarelo = al_map_rgb(255, 220, 0);
    ALLEGRO_COLOR laranja = al_map_rgb(255, 140, 0);
    ALLEGRO_COLOR verde = al_map_rgb(0, 255, 120);
    ALLEGRO_COLOR azul = al_map_rgb(20, 45, 100);

    char letras[3] = { 'A', 'B', 'C' };
    int tela = 0;             // 0 = pergunta, 1 = mostra a resposta
    int resposta = -1;        // -1 = nao respondeu, 0 = A, 1 = B, 2 = C
    int retorno = -1;         // -1 = continua nessa tela
    char texto[200];

    while (retorno == -1) {

        // ---------- desenhar ----------
        al_clear_to_color(azul);
        sprintf(texto, "PERGUNTA - %s", esporte);
        escrever(fonteGrande, laranja, 40, texto);
        escrever(fonte, branco, 150, pergunta);

        // as 3 alternativas
        for (int i = 0; i < 3; i++) {
            ALLEGRO_COLOR corBotao = al_map_rgb(40, 90, 170);
            if (tela == 1 && i == correta) {
                corBotao = al_map_rgb(20, 140, 70);    // certa fica verde
            }
            else if (tela == 1 && i == resposta) {
                corBotao = al_map_rgb(170, 40, 40);    // errada fica vermelha
            }
            al_draw_filled_rounded_rectangle(200, 210 + i * 80, 1000, 270 + i * 80, 12, 12, corBotao);
            sprintf(texto, "%c) %s", letras[i], opcoes[i]);
            escrever(fonte, branco, 225 + i * 80, texto);
        }

        if (tela == 0) {
            escrever(fonte, verde, 500, "Aperte A, B ou C");
            escrever(fonte, branco, 690, "ESC/BACKSPACE para o menu");
        }
        else {
            if (resposta == correta) {
                escrever(fonte, verde, 480, "CERTO!");
            }
            else {
                escrever(fonte, al_map_rgb(255, 80, 80), 480, "Nao foi dessa vez.");
            }
            escrever(fonte, amarelo, 525, explicacao);
            escrever(fonte, verde, 600, "ENTER para voltar ao menu");
        }
        al_flip_display();

        // ---------- esperar o jogador ----------
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            retorno = FECHAR_JOGO;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            int tecla = evento.keyboard.keycode;

            if (tecla == ALLEGRO_KEY_ESCAPE || tecla == ALLEGRO_KEY_BACKSPACE) {
                retorno = VOLTAR_MENU;
            }
            else if (tela == 0) {
                if (tecla == ALLEGRO_KEY_A) resposta = 0;
                if (tecla == ALLEGRO_KEY_B) resposta = 1;
                if (tecla == ALLEGRO_KEY_C) resposta = 2;

                if (resposta != -1) {
                    tela = 1;   // mostra se acertou
                }
            }
            else if (tecla == ALLEGRO_KEY_ENTER) {
                retorno = VOLTAR_MENU;
            }
        }
    }

    al_destroy_event_queue(fila);
    return retorno;
}
