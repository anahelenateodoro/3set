#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <stdio.h>
#include "ana_sprites.h"
#include "rico_sprites.h"

#define LARGURA 1000
#define ALTURA 600
#define CHAO 470
#define INICIO 100
#define TROCA 1500
#define CHEGADA 2900
// Cada atualizacao representa 1/60 de segundo.
#define TEMPO_FRAME (1.0f / 60)
#define NUM_BARREIRAS 4

static const float barreiras[NUM_BARREIRAS] = {600, 1100, 2000, 2500};
static float posicao;
static float salto;
static float velocidadeSalto;
static float tempo;
static int atleta; // 0 = Ana, 1 = Rico
static int erros;
static bool derrubadas[NUM_BARREIRAS];
static bool anaCorrendo;
static bool ricoCorrendo;
static bool terminou;

static ALLEGRO_BITMAP *anaParada;
static ALLEGRO_BITMAP *anaCorrida;
static ALLEGRO_BITMAP *anaPulando;
static ALLEGRO_BITMAP *ricoParado;
static ALLEGRO_BITMAP *ricoCorrida;
static ALLEGRO_BITMAP *ricoPulando;

static bool atleta_correndo(void) {
    if (atleta == 0) {
        return anaCorrendo;
    } else {
        return ricoCorrendo;
    }
}

// Verifica uma regra de cada vez antes de iniciar o salto.
static void tentar_saltar(int tecla) {
    if (terminou == true) {
        return;
    }
    if (salto > 0) {
        return;
    }

    if (atleta == 0) {
        if (tecla == ALLEGRO_KEY_W) {
            velocidadeSalto = 450;
        }
    } else {
        if (tecla == ALLEGRO_KEY_UP) {
            velocidadeSalto = 450;
        }
    }
}

static void tentar_passar_bastao(void) {
    if (terminou == true) {
        return;
    }
    if (atleta != 0) {
        return;
    }
    if (salto > 0) {
        return;
    }
    if (posicao < TROCA - 100) {
        return;
    }
    if (posicao > TROCA + 100) {
        return;
    }

    atleta = 1; // Agora e a vez do Rico.
}

static void reiniciar(void) {
    posicao = INICIO;
    salto = 0;
    velocidadeSalto = 0;
    tempo = 0;
    atleta = 0;
    erros = 0;
    terminou = false;
    anaCorrendo = false;
    ricoCorrendo = false;
    for (int i = 0; i < NUM_BARREIRAS; i++) {
        derrubadas[i] = false;
    }
}

static void atualizar(void) {
    if (terminou == true) {
        return;
    }
    if (posicao > INICIO || atleta_correndo()) {
        tempo = tempo + TEMPO_FRAME;
    }
    if (salto > 0 || velocidadeSalto > 0) {
        salto = salto + velocidadeSalto * TEMPO_FRAME;
        velocidadeSalto = velocidadeSalto - 1200 * TEMPO_FRAME;
        if (salto <= 0) {
            salto = 0;
            velocidadeSalto = 0;
        }
    }
    if (atleta_correndo()) {
        posicao = posicao + 300 * TEMPO_FRAME;
    }
    // Ana para no fim da zona ate passar o bastao para Rico.
    if (atleta == 0 && posicao > TROCA + 100) {
        posicao = TROCA + 100;
    }
    for (int i = 0; i < NUM_BARREIRAS; i++) {
        float inicioBarreira = barreiras[i] - 18;
        float fimBarreira = barreiras[i] + 18;
        bool pertoDaBarreira = posicao >= inicioBarreira && posicao <= fimBarreira;
        bool saltoBaixo = salto < 65;

        if (derrubadas[i] == false && pertoDaBarreira && saltoBaixo) {
            derrubadas[i] = true;
            erros = erros + 1;
        }
    }
    if (posicao >= CHEGADA) {
        posicao = CHEGADA;
        terminou = true;
    }
}

static void desenhar(ALLEGRO_FONT *fonte) {
    ALLEGRO_COLOR branco = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR azul = al_map_rgb(25, 45, 65);
    float camera = posicao - 250;
    if (camera < 0) {
        camera = 0;
    }
    if (camera > CHEGADA - 750) {
        camera = CHEGADA - 750;
    }
    al_clear_to_color(al_map_rgb(190, 225, 245));
    al_draw_filled_rectangle(0, 370, LARGURA, ALTURA, al_map_rgb(85, 160, 95));
    al_draw_filled_rectangle(0, 410, LARGURA, 520, al_map_rgb(185, 85, 65));
    al_draw_line(0, CHAO, LARGURA, CHAO, branco, 3);
    al_draw_filled_rectangle(TROCA - 100 - camera, 410,
        TROCA + 100 - camera, 520, al_map_rgb(235, 185, 50));
    al_draw_text(fonte, azul, TROCA - camera, 385,
        ALLEGRO_ALIGN_CENTER, "ZONA DE TROCA (E)");
    al_draw_line(CHEGADA - camera, 410, CHEGADA - camera, 520, branco, 8);
    al_draw_text(fonte, azul, CHEGADA - camera, 385, ALLEGRO_ALIGN_CENTER, "CHEGADA");
    for (int i = 0; i < NUM_BARREIRAS; i++) {
        float x = barreiras[i] - camera;
        if (derrubadas[i]) {
            al_draw_line(x - 25, CHAO, x + 25, CHAO, branco, 6);
        } else {
            al_draw_line(x - 20, CHAO, x - 20, CHAO - 65, azul, 4);
            al_draw_line(x + 20, CHAO, x + 20, CHAO - 65, azul, 4);
            al_draw_line(x - 25, CHAO - 65, x + 25, CHAO - 65, branco, 7);
        }
    }
    // Escolhe a imagem de acordo com o atleta e seu movimento.
    ALLEGRO_BITMAP *sprite;
    const char *nomeAtleta;
    if (atleta == 0) {
        nomeAtleta = "Ana";
        sprite = anaParada;
        if (salto > 0) {
            sprite = anaPulando;
        } else if (anaCorrendo && terminou == false) {
            sprite = anaCorrida;
        }
    } else {
        nomeAtleta = "Rico";
        sprite = ricoParado;
        if (salto > 0) {
            sprite = ricoPulando;
        } else if (ricoCorrendo && terminou == false) {
            sprite = ricoCorrida;
        }
    }
    float x = posicao - camera - 45;
    float y = CHAO - 135 - salto;
    int largura = al_get_bitmap_width(sprite);
    int altura = al_get_bitmap_height(sprite);

    al_draw_scaled_bitmap(sprite, 0, 0, largura, altura, x, y, 90, 135, 0);

    ALLEGRO_COLOR amarelo = al_map_rgb(255, 220, 20);
    float bastaoX = x + 70;
    float bastaoY = y + 70;
    al_draw_line(bastaoX, bastaoY, bastaoX + 20, bastaoY - 15, amarelo, 5);

    al_draw_text(fonte, azul, 20, 20, 0, "DuoFit - Atletismo");
    al_draw_text(fonte, azul, 20, 45, 0, nomeAtleta);
    al_draw_textf(fonte, azul, 20, 70, 0, "Tempo: %.2f s", tempo);
    al_draw_textf(fonte, azul, 20, 95, 0, "Barreiras derrubadas: %d", erros);

    al_draw_text(fonte, branco, 20, 540, 0, "Ana: D corre, W pula | Rico: direita corre, cima pula");
    al_draw_text(fonte, branco, 20, 565, 0, "E: passa bastao na zona amarela | R: reinicia | ESC: sai");

    if (terminou == true) {
        al_draw_text(fonte, azul, 350, 250, 0, "Chegaram! R para jogar novamente.");
    }
}

void jogar_atletismo(void) {
    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA, ALTURA);
    if (janela == NULL) {
        fprintf(stderr, "Erro ao criar janela.\n");
        return;
    }
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(TEMPO_FRAME);
    ALLEGRO_FONT *fonte = al_create_builtin_font();
    ALLEGRO_BITMAP *atlasAna = carregar_atlas_ana();
    ALLEGRO_BITMAP *atlasRico = carregar_atlas_rico();
    anaParada = recortar_ana(atlasAna, ANA_PARADA);
    anaCorrida = recortar_ana(atlasAna, ANA_ANDANDO1);
    anaPulando = recortar_ana(atlasAna, ANA_PULANDO);
    ricoParado = recortar_rico(atlasRico, RICO_PARADO);
    ricoCorrida = recortar_rico(atlasRico, RICO_ANDANDO);
    ricoPulando = recortar_rico(atlasRico, RICO_PULANDO);

    bool carregou = true;
    if (fila == NULL || timer == NULL || fonte == NULL) {
        carregou = false;
    }
    if (anaParada == NULL || anaCorrida == NULL || anaPulando == NULL) {
        carregou = false;
    }
    if (ricoParado == NULL || ricoCorrida == NULL || ricoPulando == NULL) {
        carregou = false;
    }

    if (carregou == false) {
        fprintf(stderr, "Erro ao carregar recursos. Execute na pasta do projeto.\n");
    } else {
        al_set_window_title(janela, "DuoFit - MVP");
        al_register_event_source(fila, al_get_display_event_source(janela));
        al_register_event_source(fila, al_get_keyboard_event_source());
        al_register_event_source(fila, al_get_timer_event_source(timer));
        reiniciar();
        al_start_timer(timer);
        bool rodando = true;
        bool redesenhar = true;
        while (rodando) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila, &evento);
            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                rodando = false;
            }
            if (evento.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT) {
                anaCorrendo = false;
                ricoCorrendo = false;
            }
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                int tecla = evento.keyboard.keycode;
                if (tecla == ALLEGRO_KEY_ESCAPE) {
                    rodando = false;
                }
                if (tecla == ALLEGRO_KEY_R) {
                    reiniciar();
                }
                if (tecla == ALLEGRO_KEY_D) {
                    anaCorrendo = true;
                }
                if (tecla == ALLEGRO_KEY_RIGHT) {
                    ricoCorrendo = true;
                }
                tentar_saltar(tecla);
                if (tecla == ALLEGRO_KEY_E) {
                    tentar_passar_bastao();
                }
            }
            if (evento.type == ALLEGRO_EVENT_KEY_UP) {
                if (evento.keyboard.keycode == ALLEGRO_KEY_D) {
                    anaCorrendo = false;
                }
                if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                    ricoCorrendo = false;
                }
            }
            if (evento.type == ALLEGRO_EVENT_TIMER) {
                atualizar();
                redesenhar = true;
            }
            if (rodando && redesenhar && al_is_event_queue_empty(fila)) {
                desenhar(fonte);
                al_flip_display();
                redesenhar = false;
            }
        }
    }

    if (anaParada != NULL) {
        al_destroy_bitmap(anaParada);
    }
    if (anaCorrida != NULL) {
        al_destroy_bitmap(anaCorrida);
    }
    if (anaPulando != NULL) {
        al_destroy_bitmap(anaPulando);
    }
    if (ricoParado != NULL) {
        al_destroy_bitmap(ricoParado);
    }
    if (ricoCorrida != NULL) {
        al_destroy_bitmap(ricoCorrida);
    }
    if (ricoPulando != NULL) {
        al_destroy_bitmap(ricoPulando);
    }
    if (atlasAna != NULL) {
        al_destroy_bitmap(atlasAna);
    }
    if (atlasRico != NULL) {
        al_destroy_bitmap(atlasRico);
    }
    if (fonte != NULL) {
        al_destroy_font(fonte);
    }
    if (timer != NULL) {
        al_destroy_timer(timer);
    }
    if (fila != NULL) {
        al_destroy_event_queue(fila);
    }
    al_destroy_display(janela);
}
