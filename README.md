# Jogo 3d FindBalls - Computacao-Grafica

### Principais Implementações 

gamedata.hpp :

Estrutura que define o estado atual do jogo

```
#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class State { Playing, GameOver, Win, Init, Menu };

struct GameData {
  State m_state{State::Init};
  };

#endif 
```

m_state pode ser:

    Playing : aplicação em modo jogo
    GameOver : aplicação na tela de finalização do jogo com a mensagem "Você Perdeu" e botão para jogar novamente, neste caso a pessoa e as bolas não são exibidas.
    Win : aplicação na tela de finalização do jogo com a mensagem "Você Ganhou" e botão para jogar novamente, neste caso a pessoa e as bolas não são exibidas.
    Init : aplicação na tela de inicialização aparecendo botão com a mensagem "Jogar".
    Menu : aplicação de escolha da dificuldade do jogo com os botões : Fácil, Médio e Difícil.
