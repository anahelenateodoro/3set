#include "duofit.h"
#include <stdlib.h>   

// telas da fase
#define REGRAS    1
#define HANDEBOL  2
#define VITORIA   3


#define CHAO    630    // y do chao
#define AREA    780    // x da linha dos 6 metros
#define GOL_X   1110   // x da trave
#define GOL_Y   330    // y do travessao

// tamanhos dos desenhos
#define RICO_L     140
#define RICO_A     210
#define GOLEIRO_L  130
#define GOLEIRO_A  200
#define BOLA_T     42

int jogar_handebol(void) {
    int retorno = VOLTAR_MENU;

    // fila de eventos: janela, teclado e timer de 60 FPS
    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    al_register_event_source(fila, al_get_display_event_source(al_get_current_display()));
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_timer_event_source(timer));

    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255), preto = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR amarelo = al_map_rgb(255, 220, 0), laranja = al_map_rgb(255, 140, 0);
    ALLEGRO_COLOR verde = al_map_rgb(0, 255, 120);

    char* regras[5] = {
        "1. Com a bola na mao, no maximo 3 passos.",
        "2. Aperte S para quicar a bola e ganhar mais 3 passos.",
        "3. Nao pise na area do goleiro segurando a bola.",
        "4. Mas pode pular na area e arremessar antes de cair!",
        "5. A barra de forca define a altura do arremesso." };

    int estado = REGRAS;

    // Rico
    float ricoX = 150, ricoY = CHAO - RICO_A, ricoVelY = 0;
    int direcao = 1;           // 1 = direita, -1 = esquerda
    int tempoAndando = 0;      // > 0 mostra a imagem "andando"
    bool noChao = true;
    bool jaMexeuNoAr = false;  // no ar so pode mudar de lugar 1 vez

    // bola
    bool comBola = true;
    int passos = 0, tempoDrible = 0;   // tempoDrible > 0 = bola quicando
    float bolaX = 0, bolaY = 0, bolaVelX = 0, bolaVelY = 0;
    float forca = 0, variacaoForca = 2;   // barra de forca vai de 0 a 100 e volta

    // goleira (Ana)
    float goleiroX = 960, goleiroY = CHAO - GOLEIRO_A, goleiroVelX = -2, goleiroVelY = 0;
    bool goleiroDecidiu = false;   // ja sorteou se pula nesse arremesso?

    // placar
    int gols = 0, arremessos = 0;
    int infracao = 0;          // 1 = mais de 3 passos, 2 = pisou na area
    char* resultado = NULL;    // "GOOOL!" etc. NULL = sem mensagem
    int tempoResultado = 0;
    bool reiniciar = true;     // volta tudo para a posicao inicial

    bool rodando = true, redesenhar = false;
    char texto[100];
    al_start_timer(timer);

    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
            retorno = FECHAR_JOGO;
        }

        // ---------- 1) TECLADO ----------
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            int tecla = evento.keyboard.keycode;

            if (tecla == ALLEGRO_KEY_ESCAPE || tecla == ALLEGRO_KEY_BACKSPACE) {
                rodando = false;   // volta para o menu
            }
            else if (estado == REGRAS && tecla == ALLEGRO_KEY_ENTER) {
                gols = 0; arremessos = 0; infracao = 0; resultado = NULL;
                reiniciar = true;
                estado = HANDEBOL;
            }
            else if (estado == VITORIA && tecla == ALLEGRO_KEY_ENTER) {
                char* opcoes[3] = { "2 passos", "3 passos", "5 passos" };
                retorno = fazer_pergunta("HANDEBOL", "Quantos passos pode dar quem esta segurando a bola?",
                    opcoes, 1, "No maximo 3 passos. Para andar mais, e preciso quicar a bola.");
                rodando = false;
            }
            else if (estado == HANDEBOL && infracao != 0) {
                // com o aviso aberto, so o ENTER funciona; pisou na area volta ao comeco
                if (tecla == ALLEGRO_KEY_ENTER) {
                    if (infracao == 2) reiniciar = true;
                    infracao = 0;
                }
            }
            else if (estado == HANDEBOL && resultado == NULL) {
                // A / D = andar (no ar nao conta passo, mas so mexe 1 vez)
                if (tecla == ALLEGRO_KEY_A || tecla == ALLEGRO_KEY_D) {
                    direcao = -1;
                    if (tecla == ALLEGRO_KEY_D) direcao = 1;

                    if (noChao == false && jaMexeuNoAr == false) {
                        ricoX = ricoX + 60 * direcao;
                        jaMexeuNoAr = true;
                    }
                    else if (noChao && comBola && passos >= 3) {
                        infracao = 1;   // regra dos 3 passos
                    }
                    else if (noChao) {
                        ricoX = ricoX + 60 * direcao;
                        tempoAndando = 8;
                        if (comBola) passos++;
                    }
                }
                // W = pular (velocidade negativa sobe)
                if (tecla == ALLEGRO_KEY_W && noChao) {
                    ricoVelY = -16; noChao = false; jaMexeuNoAr = false;
                }
                // S = quicar: zera os passos
                if (tecla == ALLEGRO_KEY_S && comBola && noChao && tempoDrible == 0) {
                    tempoDrible = 20; passos = 0;
                }
                // ESPACO = arremessar (no pulo a bola sai do punho levantado)
                if (tecla == ALLEGRO_KEY_SPACE && comBola && tempoDrible == 0) {
                    bolaX = ricoX + 105;
                    if (direcao == -1) bolaX = ricoX - 5;
                    bolaY = ricoY + 100;
                    if (noChao == false) bolaY = ricoY + 20;

                    bolaVelX = 18 * direcao;
                    bolaVelY = -1 - forca * 0.12;   // mais forca = sobe mais
                    comBola = false; goleiroDecidiu = false;
                    arremessos++;
                }
            }
        }

        // ---------- 2) ATUALIZAR (60 vezes por segundo) ----------
        if (evento.type == ALLEGRO_EVENT_TIMER) redesenhar = true;

        if (evento.type == ALLEGRO_EVENT_TIMER && estado == HANDEBOL) {
            if (reiniciar) {
                ricoX = 150; ricoY = CHAO - RICO_A; ricoVelY = 0; noChao = true; direcao = 1;
                comBola = true; passos = 0; tempoDrible = 0;
                goleiroX = 960; goleiroY = CHAO - GOLEIRO_A; goleiroVelY = 0;
                reiniciar = false;
            }

            // pulo: posicao += velocidade e velocidade += gravidade (vira uma parabola)
            ricoY = ricoY + ricoVelY;
            ricoVelY = ricoVelY + 0.8;
            if (ricoY >= CHAO - RICO_A) {
                ricoY = CHAO - RICO_A; ricoVelY = 0; noChao = true;
            }

            if (ricoX < 0) ricoX = 0;
            if (ricoX > LARGURA - RICO_L) ricoX = LARGURA - RICO_L;
            if (tempoAndando > 0) tempoAndando--;
            if (tempoDrible > 0) tempoDrible--;

            // regra da area: o meio do Rico passou da linha com a bola na mao
            if (comBola && noChao && infracao == 0 && ricoX + RICO_L / 2 > AREA) infracao = 2;

            // barra de forca
            if (comBola) {
                forca = forca + variacaoForca;
                if (forca >= 100 || forca <= 0) variacaoForca = -variacaoForca;
            }

            // goleira anda de um lado para o outro
            goleiroX = goleiroX + goleiroVelX;
            if (goleiroX < 900 || goleiroX > 975) goleiroVelX = -goleiroVelX;

            // bola voando (mesma fisica do pulo, gravidade menor)
            if (comBola == false && resultado == NULL) {
                bolaX = bolaX + bolaVelX;
                bolaY = bolaY + bolaVelY;
                bolaVelY = bolaVelY + 0.4;

                // quica no chao perdendo 40% da velocidade
                if (bolaY + BOLA_T >= CHAO) {
                    bolaY = CHAO - BOLA_T;
                    bolaVelY = -bolaVelY * 0.6;
                }

                // bola chegando: a goleira sorteia se pula (50% de chance)
                if (goleiroDecidiu == false && bolaX > goleiroX - 280) {
                    goleiroDecidiu = true;
                    if (rand() % 2 == 1) goleiroVelY = -15;
                }

                // bateu na goleira? (caixa um pouco menor que a imagem)
                if (bolaX < goleiroX + GOLEIRO_L - 25 && bolaX + BOLA_T > goleiroX + 25 &&
                    bolaY < goleiroY + GOLEIRO_A && bolaY + BOLA_T > goleiroY + 20) {
                    resultado = "DEFESA DA GOLEIRA!";
                }
                else if (bolaX + BOLA_T >= GOL_X && bolaY >= GOL_Y) {   // por baixo do travessao
                    resultado = "GOOOL!";
                    gols++;
                    tocar_som_gol();
                }
                else if (bolaX + BOLA_T >= GOL_X || bolaX < -BOLA_T) {
                    resultado = "PARA FORA!";
                }
                if (resultado != NULL) tempoResultado = 90;   // 1,5 segundo
            }

            // pulo da goleira
            if (goleiroVelY != 0) {
                goleiroY = goleiroY + goleiroVelY;
                goleiroVelY = goleiroVelY + 0.8;
                if (goleiroY >= CHAO - GOLEIRO_A) {
                    goleiroY = CHAO - GOLEIRO_A; goleiroVelY = 0;
                }
            }

            // tempo da mensagem; com 3 gols vai para a vitoria
            if (resultado != NULL) {
                tempoResultado--;
                if (tempoResultado <= 0) {
                    resultado = NULL; reiniciar = true;
                    if (gols >= 3) estado = VITORIA;
                }
            }
        }

        // ---------- 3) DESENHAR ----------
        if (rodando == false || redesenhar == false || al_is_event_queue_empty(fila) == false) continue;
        redesenhar = false;

        if (estado == REGRAS) {
            al_clear_to_color(al_map_rgb(20, 45, 100));
            escrever(fonteGrande, laranja, 50, "HANDEBOL");
            for (int i = 0; i < 5; i++) {
                if (i == 3) escrever(fonte, amarelo, 200 + i * 50, regras[i]);
                else escrever(fonte, branco, 200 + i * 50, regras[i]);
            }
            escrever(fonte, verde, 470, "Faca 3 gols para vencer.");
            desenhar(bola, LARGURA / 2 - 40, 560, 80, 80, 0);
            escrever(fonte, branco, ALTURA - 60, "ENTER para comecar  |  ESC/BACKSPACE para o menu");
        }
        else if (estado == VITORIA) {
            // pontos: 30 menos 2 por arremesso que nao foi gol (minimo 10)
            int pontos = 30 - 2 * (arremessos - gols);
            if (pontos < 10) pontos = 10;

            al_clear_to_color(al_map_rgb(20, 100, 60));
            escrever(fonteGrande, branco, 150, "PARABENS!");
            sprintf(texto, "Gols: %d  |  Arremessos: %d", gols, arremessos);
            escrever(fonte, amarelo, 250, texto);
            sprintf(texto, "Pontos da fase: %d  (30 - 2 por arremesso errado)", pontos);
            escrever(fonte, branco, 290, texto);
            desenhar(ricoPulando, LARGURA / 2 - 90, 330, 180, 270, 0);
            escrever(fonte, branco, ALTURA - 60, "ENTER: responder a pergunta final  |  ESC: menu");
        }
        else {
            // quadra: chao azul, area laranja e gol vermelho
            al_clear_to_color(al_map_rgb(215, 235, 245));
            al_draw_filled_rectangle(0, CHAO, AREA, ALTURA, al_map_rgb(40, 90, 170));
            al_draw_filled_rectangle(AREA, CHAO, LARGURA, ALTURA, al_map_rgb(240, 140, 40));
            al_draw_line(AREA, CHAO, AREA, CHAO + 45, branco, 5);
            al_draw_filled_rectangle(GOL_X - 8, GOL_Y, GOL_X, CHAO, al_map_rgb(220, 30, 30));
            al_draw_filled_rectangle(GOL_X - 8, GOL_Y - 8, LARGURA, GOL_Y, al_map_rgb(220, 30, 30));

            sprintf(texto, "Gols: %d / 3    Passos: %d / 3", gols, passos);
            escrever(fonte, preto, 20, texto);

            // barra de forca: de verde (fraco) para vermelho (forte)
            al_draw_filled_rectangle(30, 30, 30 + forca * 2, 55, al_map_rgb(forca * 2.5, 200 - forca * 1.5, 40));
            al_draw_rectangle(30, 30, 230, 55, preto, 2);

            // goleira (as imagens olham para a direita, entao espelho quando precisa)
            if (goleiroVelY != 0) desenhar(anaPulando, goleiroX, goleiroY, GOLEIRO_L, GOLEIRO_A, ALLEGRO_FLIP_HORIZONTAL);
            else if (goleiroVelX < 0) desenhar(anaAndando, goleiroX, goleiroY, GOLEIRO_L, GOLEIRO_A, ALLEGRO_FLIP_HORIZONTAL);
            else desenhar(anaAndando, goleiroX, goleiroY, GOLEIRO_L, GOLEIRO_A, 0);

            // Rico
            ALLEGRO_BITMAP* imagem = ricoParado;
            if (noChao == false) imagem = ricoPulando;
            else if (tempoAndando > 0) imagem = ricoAndando;
            int espelhar = 0;
            if (direcao == -1) espelhar = ALLEGRO_FLIP_HORIZONTAL;
            desenhar(imagem, ricoX, ricoY, RICO_L, RICO_A, espelhar);

            // bola na mao (quicando: de 20 a 11 desce, de 10 a 0 sobe)
            if (comBola) {
                float maoX = ricoX + 102, maoY = ricoY + 100;
                if (direcao == -1) maoX = ricoX - 3;
                if (noChao == false) maoY = ricoY + 20;
                float distanciaChao = (CHAO - BOLA_T) - maoY;
                if (tempoDrible > 10) maoY = maoY + distanciaChao * (20 - tempoDrible) / 10.0;
                else if (tempoDrible > 0) maoY = maoY + distanciaChao * tempoDrible / 10.0;
                desenhar(bola, maoX, maoY, BOLA_T, BOLA_T, 0);
            }
            else if (resultado == NULL) {
                desenhar(bola, bolaX, bolaY, BOLA_T, BOLA_T, 0);   // bola voando
            }

            escrever(fonte, branco, ALTURA - 45, "A/D andar | W pular | S quicar | ESPACO arremessar | ESC/BACKSPACE menu");
            if (resultado != NULL) escrever(fonteGrande, laranja, 200, resultado);

            // aviso de infracao
            if (infracao != 0) {
                al_draw_filled_rectangle(280, 235, 920, 515, al_map_rgb(70, 20, 20));
                al_draw_rectangle(280, 235, 920, 515, branco, 3);
                escrever(fonte, al_map_rgb(255, 80, 80), 280, "INFRACAO!");
                if (infracao == 1) {
                    escrever(fonte, branco, 335, "Mais de 3 passos segurando a bola.");
                    escrever(fonte, amarelo, 380, "Quique a bola (S) para andar mais.");
                }
                else {
                    escrever(fonte, branco, 335, "Voce pisou na area com a bola.");
                    escrever(fonte, amarelo, 380, "Pule e arremesse antes de cair!");
                }
                escrever(fonte, verde, 450, "ENTER para continuar");
            }
        }
        al_flip_display();
    }

    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    return retorno;
}
