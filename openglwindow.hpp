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