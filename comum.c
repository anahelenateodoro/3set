#include "duofit.h"

ALLEGRO_FONT* fonte = NULL;
ALLEGRO_FONT* fonteGrande = NULL;
ALLEGRO_FONT* fonteMenu = NULL;

ALLEGRO_BITMAP* imagemAna = NULL;    
ALLEGRO_BITMAP* imagemRico = NULL;  
ALLEGRO_BITMAP* bola = NULL;
ALLEGRO_BITMAP* anaAndando = NULL;
ALLEGRO_BITMAP* anaPulando = NULL;
ALLEGRO_BITMAP* ricoParado = NULL;
ALLEGRO_BITMAP* ricoAndando = NULL;
ALLEGRO_BITMAP* ricoPulando = NULL;

ALLEGRO_SAMPLE* somGol = NULL;  

ALLEGRO_FONT* abrir_fonte(int tamanho, bool negrito) {
    ALLEGRO_FONT* f;
    if (negrito) {
        f = al_load_ttf_font("C:/Windows/Fonts/arialbd.ttf", tamanho, 0);
        if (f == NULL) {
            f = al_load_ttf_font("/System/Library/Fonts/Supplemental/Arial Bold.ttf", tamanho, 0);
        }
    }
    else {
        f = al_load_ttf_font("C:/Windows/Fonts/arial.ttf", tamanho, 0);
        if (f == NULL) {
            f = al_load_ttf_font("/System/Library/Fonts/Supplemental/Arial.ttf", tamanho, 0);
        }
    }
    if (f == NULL) {
        f = al_create_builtin_font();
    }
    return f;
}

bool carregar_recursos(void) {
    fonte = abrir_fonte(24, false);
    fonteGrande = abrir_fonte(56, true);
    fonteMenu = abrir_fonte(32, false);

    imagemAna = al_load_bitmap("assets/ana_sprites.png");
    imagemRico = al_load_bitmap("assets/rico_sprites.png");
    bola = al_load_bitmap("assets/bola.png");
    if (imagemAna == NULL || imagemRico == NULL || bola == NULL) {
        return false;
    }

    // Recorta cada pose da folha de imagens.
    // al_create_sub_bitmap nao copia a imagem, so aponta para um pedaco dela.
    // Ana (goleira): cada pose tem 256 x 384
    anaAndando = al_create_sub_bitmap(imagemAna, 256, 0, 256, 384);
    anaPulando = al_create_sub_bitmap(imagemAna, 256, 384, 256, 384);
    // Rico: as 3 poses de 256 x 384 ficam na linha de cima
    ricoParado = al_create_sub_bitmap(imagemRico, 0, 0, 256, 384);
    ricoAndando = al_create_sub_bitmap(imagemRico, 256, 0, 256, 384);
    ricoPulando = al_create_sub_bitmap(imagemRico, 512, 0, 256, 384);

    // som do gol (se nao tiver placa de som ou o arquivo, segue sem som)
    if (al_install_audio() && al_init_acodec_addon() && al_reserve_samples(1)) {
        somGol = al_load_sample("assets/gol.wav");
    }
    if (somGol == NULL) {
        printf("Aviso: o jogo vai rodar sem som.\n");
    }
    return true;
}


void liberar_recursos(void) {
    if (anaAndando != NULL) al_destroy_bitmap(anaAndando);
    if (anaPulando != NULL) al_destroy_bitmap(anaPulando);
    if (ricoParado != NULL) al_destroy_bitmap(ricoParado);
    if (ricoAndando != NULL) al_destroy_bitmap(ricoAndando);
    if (ricoPulando != NULL) al_destroy_bitmap(ricoPulando);
    if (imagemAna != NULL) al_destroy_bitmap(imagemAna);
    if (imagemRico != NULL) al_destroy_bitmap(imagemRico);
    if (bola != NULL) al_destroy_bitmap(bola);
    if (somGol != NULL) {
        al_stop_samples();
        al_destroy_sample(somGol);
    }
    if (fonte != NULL) al_destroy_font(fonte);
    if (fonteGrande != NULL) al_destroy_font(fonteGrande);
    if (fonteMenu != NULL) al_destroy_font(fonteMenu);
}

void escrever(ALLEGRO_FONT* f, ALLEGRO_COLOR cor, float y, char* mensagem) {
    al_draw_text(f, cor, LARGURA / 2, y, ALLEGRO_ALIGN_CENTER, mensagem);
}

// Desenha a imagem na posicao (x, y) com a largura e altura que eu quiser.
// flags = 0 desenha normal, ALLEGRO_FLIP_HORIZONTAL desenha espelhado.
void desenhar(ALLEGRO_BITMAP* imagem, float x, float y, float largura, float altura, int flags) {
    float larguraOriginal = al_get_bitmap_width(imagem);
    float alturaOriginal = al_get_bitmap_height(imagem);
    al_draw_scaled_bitmap(imagem, 0, 0, larguraOriginal, alturaOriginal, x, y, largura, altura, flags);
}

void tocar_som_gol(void) {
    if (somGol != NULL) {
        al_stop_samples();  
                al_play_sample(somGol, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}
