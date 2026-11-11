
#ifndef DUOFIT_H
#define DUOFIT_H

#define _CRT_SECURE_NO_WARNINGS 

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdbool.h>

#define LARGURA 1200
#define ALTURA  750

#define VOLTAR_MENU  0
#define FECHAR_JOGO  2

// Fontes, imagens e som sao carregados uma vez so no comeco do jogo
// e todas as fases usam os mesmos (assim gasta menos memoria).
// O "extern" quer dizer que a variavel foi criada em outro arquivo (comum.c).
extern ALLEGRO_FONT* fonte;          // texto normal
extern ALLEGRO_FONT* fonteGrande;    // titulos
extern ALLEGRO_FONT* fonteMenu;      // botoes do menu

extern ALLEGRO_BITMAP* bola;
extern ALLEGRO_BITMAP* anaAndando;
extern ALLEGRO_BITMAP* anaPulando;
extern ALLEGRO_BITMAP* ricoParado;
extern ALLEGRO_BITMAP* ricoAndando;
extern ALLEGRO_BITMAP* ricoPulando;


bool carregar_recursos(void);
void liberar_recursos(void);
void escrever(ALLEGRO_FONT* f, ALLEGRO_COLOR cor, float y, char* mensagem);
void desenhar(ALLEGRO_BITMAP* imagem, float x, float y, float largura, float altura, int flags);
void tocar_som_gol(void);


int fazer_pergunta(char* esporte, char* pergunta, char* opcoes[3], int correta, char* explicacao);

int mostrar_menu(void);
int jogar_handebol(void);

#endif
