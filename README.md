# DuoFit — MVP

Corrida cooperativa de atletismo para duas pessoas no mesmo teclado.
Ana corre o primeiro trecho, passa o bastao na zona amarela e Rico termina.
O objetivo e chegar ao fim, saltando quatro barreiras. O resultado mostra
tempo total e barreiras derrubadas.

## Executar

Requer compilador C, `make`, `pkg-config` e Allegro 5 com addons
primitives, image e font. Na pasta do projeto:

```sh
make
make run
```

- Ana: segurar **D** para correr e **W** para saltar.
- Passagem: **E**, no chao e dentro da zona amarela.
- Rico: segurar **seta direita** para correr e **seta cima** para saltar.
- **R** reinicia; **Esc** encerra.

O MVP inclui uma pista, dois personagens, salto, passagem de bastao e resultado.
Aulas, quiz, menu de modalidades e animacoes avancadas ficam fora desta versao.
Os sprites sao carregados de `assets/`; execute a partir da pasta do projeto.
