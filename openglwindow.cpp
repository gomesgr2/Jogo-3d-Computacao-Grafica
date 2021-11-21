#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

// Explicit specialization of std::hash for Vertex
namespace std {
template <>
struct hash<Vertex> {
  size_t operator()(Vertex const& vertex) const noexcept {
    const std::size_t h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};
}  // namespace std

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_q) m_truckSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_e) m_truckSpeed = 1.0f;
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_q && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_e && m_truckSpeed > 0) m_truckSpeed = 0.0f;
  }
}

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

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  const GLint colorLoc{abcg::glGetUniformLocation(m_program, "color")};

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);

  abcg::glBindVertexArray(m_VAO);

  for (auto& ball : m_balls) {
    glm::mat4 ball_model{1.0f};
    // Draw ball
    ball_model = glm::translate(ball_model,
                                glm::vec3(ball.position_x, 0, ball.position_z));
    ball_model =
        glm::rotate(ball_model, glm::radians(90.0f), glm::vec3(0, 1, 0));
    ball_model = glm::scale(ball_model, glm::vec3(0.1f));

    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &ball_model[0][0]);

    if (ball.wasFound) {
      abcg::glUniform4f(colorLoc, 0.0f, 0.8f, 1.0f, 1.0f);
    } else {
      abcg::glUniform4f(colorLoc, 1.0f, 0.25f, 0.25f, 1.0f);
    }

    m_ball_model.render(-1);
  }

  glm::mat4 house_model{1.0f};
  house_model = glm::translate(house_model, glm::vec3(0.5f, 0.0f, 5.0f));
  house_model =
      glm::rotate(house_model, glm::radians(90.0f), glm::vec3(0, 1, 0));
  house_model = glm::scale(house_model, glm::vec3(5.0f));
  abcg::glUniform4f(colorLoc, 0.99f, 0.77f, 0.53f, 0.0f);
  m_house_model.render(-1);

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  const auto size{ImVec2(500, 500)};
  const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                             (m_viewportHeight - size.y) / 2.0f)};
  ImGui::SetNextWindowPos(position);
  ImGui::SetNextWindowSize(size);
  ImGuiWindowFlags flags{};

  // As flags serão relativas ao estado do jogo
  if (m_gameData.m_state == State::Playing) {
    flags = {ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar |
             ImGuiWindowFlags_NoInputs};

  } else {
    flags = {ImGuiWindowFlags_NoDecoration};
  }

  ImGui::Begin(" ", nullptr, flags);
  ImGui::PushFont(m_font);

  if (m_gameData.m_state == State::Init) {
    ImGui::Text("VOCÊ TEM 20 SEGUNDOS PARA ENCONTRAR TODAS AS BOLAS");
    if (ImGui::Button("Jogar", ImVec2(300, 80))) {
      initBalls(5);
      m_gameData.m_state = State::Menu;
    }
  } else if (m_gameData.m_state == State::Menu) {
    if (ImGui::Button("FÁCIL - 3 BOLAS", ImVec2(300, 80))) {
      m_gameData.m_state = State::Playing;
      m_game_time.restart();
      initBalls(3);
    }
    if (ImGui::Button("MÉDIO - 4 BOLAS", ImVec2(300, 80))) {
      m_gameData.m_state = State::Playing;
      m_game_time.restart();

      initBalls(4);
    }
    if (ImGui::Button("DIFÍCIL- 6 BOLAS ", ImVec2(300, 80))) {
      m_gameData.m_state = State::Playing;
      m_game_time.restart();

      initBalls(6);
    }

  } else if (m_gameData.m_state == State::Playing) {
    std::string text = std::to_string(numberOfFoundItems);
    char const* textFormat = text.c_str();
    ImGui::Text("NÚMERO DE BOLAS ENCONTRADAS :");
    ImGui::Text(textFormat);
  } else {
    const char* message =
        m_gameData.m_state == State::GameOver ? "Você Perdeu" : "Você Ganhou";
    ImGui::Text(message);
    if (ImGui::Button("Jogar Novamente", ImVec2(300, 80))) {
      numberOfFoundItems = 0;
      m_gameData.m_state = State::Menu;
    }
  }

  ImGui::PopFont();
  ImGui::End();
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  m_ball_model.terminateGL();
  m_house_model.terminateGL();
  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  if (m_gameData.m_state == State::Playing) {
    checkGameCondition();
    checkFound();
  }

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}

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

void OpenGLWindow::checkFound() {
  for (auto& ball : m_balls) {
    if (!ball.wasFound) {
      const auto distance{
          glm::distance(glm::vec3(ball.position_x, 0, ball.position_z),
                        glm::vec3(m_camera.m_eye.x, 0, m_camera.m_eye.z))};
      if (distance < 0.8f) {
        ball.wasFound = true;
        numberOfFoundItems++;
      }
    }
  }
}

void OpenGLWindow::checkGameCondition() {
  if (m_game_time.elapsed() > 20) {
    m_gameData.m_state = State::GameOver;
    m_game_time.restart();
  }

  if (numberOfFoundItems == m_balls.size()) {
    m_gameData.m_state = State::Win;
    m_game_time.restart();
  }
}
