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

```
#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <list>
#include <vector>
#include <random>
#include "abcg.hpp"
#include "camera.hpp"
#include "gamedata.hpp"
#include "model.hpp"
#include <imgui.h>


struct BallPosition {
  float position_x;
  float position_z;
  bool wasFound;
};

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};

  Camera m_camera;
  GameData m_gameData;
  Model m_ball_model;
  Model m_house_model;

  
  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-2.0f, 2.0f};
  std::list<BallPosition> m_balls;
  abcg::ElapsedTimer m_game_time;
  
  int numberOfFoundItems = 0;

  ImFont* m_font{};

  void update();
  void checkGameCondition();
  void initBalls(int quantity);
  void checkFound();
};

#endif

```


BallPosition - define as propriedades das bolas que terão uma posição x e z aleatórias (entre -2 e 2) e uma terceira propriedade que define se o bola foi encontrada pelo usuário.

```
struct BallPosition {
  float position_x;
  float position_z;
  bool wasFound;
};

```

m_balls - lista de BallPosition

```
std::list<BallPosition> m_balls;
```
  
 
numberOfFoundItems - variável que determina o número de bolas encontradas pelo usuário 

```
int numberOfFoundItems = 0;
```

Instâncias da classe Model, que irão construir a bola e casa. 

```
 Model m_ball_model;
 Model m_house_model;
```

