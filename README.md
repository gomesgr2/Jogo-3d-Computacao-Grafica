# Jogo 3d FindBalls - Computacao-Grafica

### Principais Implementações 

### gamedata.hpp :

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

### openglwindow.hpp

BallPosition - define as propriedades das bolas que terão uma posição x e z aleatórias (entre -2 e 2) e uma terceira propriedade que define se o bola foi encontrada pelo usuário.

```
struct BallPosition {
  float position_x;
  float position_z;
  bool wasFound;
};

``

m_balls - lista de BallPosition

```
std::list<BallPosition> m_balls;

```
  
numberOfFoundItems - variável que determina o número de bolas encontradas pelo usuário 

```
int numberOfFoundItems = 0;
```


