#!/bin/sh
# compila e roda o DuoFit MVP (Linux ou macOS com Allegro 5 instalado)
cd "$(dirname "$0")"
gcc -Wall main.c menu.c comum.c pergunta.c handebol.c -o programa $(pkg-config --cflags --libs \
  allegro-5 allegro_main-5 allegro_font-5 allegro_ttf-5 \
  allegro_image-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5)
./programa
