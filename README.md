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

### openglwindow.cpp

**OpenGL::initializeGL**

```
void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "lookat.vert",
                                    getAssetsPath() + "lookat.frag");

  // Load model
  m_ball_model.loadObj(getAssetsPath() + "soccer ball.obj", false);
  m_house_model.loadObj(getAssetsPath() + "cottage_obj.obj", false);

  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_ball_model.setupVAO(m_program);
  m_house_model.setupVAO(m_program);

  resizeGL(getWindowSettings().width, getWindowSettings().height);
}
```
**OpenGL::initBalls**

Primeiramente removemos os atributos da lista de bolas (m_balls), em seguida atribuimos o tamanho da lista de acordo com a quantidade de bolas (essa quantidade será escolhida pelo usuário, por fim populamos a lista, nessa parte adicionamos nos atribuitos position_x e position_z valores aleatórios entre -2 e 2 e adicionamos no atributo wasFound o valor false, tendo em vista que no inicio nenhuma bola foi encontrada.

```
void OpenGLWindow::initBalls(int quantity) {
  m_balls.clear();
  m_balls.resize(quantity);
  auto& re{m_randomEngine};
  for (auto& ball : m_balls) {
    ball.position_x = m_randomDist(re);
    ball.position_z = m_randomDist(re);
    ball.wasFound = false;
  }
}

```

