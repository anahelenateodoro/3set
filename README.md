# DuoFit - MVP

Versão mínima do DuoFit para apresentação: o menu e a fase de **handebol**,
com a pergunta do final.
Projeto Integrador II - Senac. Equipe: Nathali, Ana Helena e Gabriel.

## O que o MVP mostra

```
MENU -> Regras -> Jogo -> Vitória (3 gols) -> Pergunta -> MENU
```

- **Mecânica:** andar, pular, quicar e arremessar, com barra de força.
- **Regras do handebol no jogo:** no máximo 3 passos com a bola, não pisar
  na área com a bola, pular na área e arremessar antes de cair.
- **Goleira:** anda de um lado para o outro e às vezes pula (sorteio com `rand`).
- **Física:** o pulo e a bola seguem uma parábola
  (`posicao = posicao + velocidade` e `velocidade = velocidade + gravidade`).
- **Parte educativa:** pergunta sobre a regra dos passos no final.
- **Pontos da fase:** 30 menos 2 por arremesso errado (mínimo 10), mostrados na tela de vitória.
- **Som** de comemoração no gol.

| Tecla | Ação |
| --- | --- |
| A / D | andar |
| W | pular |
| S | quicar a bola |
| ESPAÇO | arremessar |
| ENTER | continuar |
| ESC / BACKSPACE | voltar ao menu |

## Próximos passos (versão completa)

- Fase de **atletismo** (revezamento com barreiras).
- Fase de **basquete** (cestas de 2 e 3 pontos).
- Pontos somados entre as três fases.

## Arquivos

| Arquivo | O que faz |
| --- | --- |
| `main.c` | Liga o Allegro, abre a janela e chama o menu e a fase |
| `duofit.h` | Constantes, variáveis globais e funções que todos os arquivos usam |
| `comum.c` | Carrega fontes, imagens e som; funções `escrever` e `desenhar` |
| `menu.c` | Menu |
| `handebol.c` | Fase de handebol |
| `pergunta.c` | Pergunta do final |
| `assets/` | Imagens e som |

## Visual Studio (Windows)

1. Crie um projeto **C++ vazio** e adicione os 5 arquivos `*.c` e o `duofit.h`.
2. Instale o pacote NuGet **Allegro**.
3. Propriedades > **Allegro 5** > Add-ons: **Image, TTF, Font, Primitives, Audio e Audio Codec**.
4. Coloque a pasta `assets/` ao lado do arquivo `.vcxproj`.
5. **Recompilar Solução** e **F5**.
