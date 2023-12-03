#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

#include "camera.hpp"
#include "model.hpp"


class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;

private:
  glm::ivec2 m_viewportSize{};

  Model m_model;
  GLuint m_program{};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};

  Camera m_camera;
  float m_dollySpeed{};
  float m_truckSpeed{};
  float m_panSpeed{};
  float m_angle{};
  float m_angle_increase{};
  float m_height{};
  float m_maxHeight{};
  float m_uppingScale{};
  float m_rotateSpeed{};
  float m_raio{};
  bool m_upping{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

  // Skybox
  std::string const m_skyShaderName{"skybox"};
  GLuint m_skyVAO{};
  GLuint m_skyVBO{};
  GLuint m_skyProgram{};

  // clang-format off
  std::array<glm::vec3, 36> const m_skyPositions{{
      // Front
      {-4, -4, +4}, {+4, -4, +4}, {+4, +4, +4},
      {-4, -4, +4}, {+4, +4, +4}, {-4, +4, +4},
      // Back
      {+4, -4, -4}, {-4, -4, -4}, {-4, +4, -4},
      {+4, -4, -4}, {-4, +4, -4}, {+4, +4, -4},
      // Right
      {+4, -4, -4}, {+4, +4, -4}, {+4, +4, +4},
      {+4, -4, -4}, {+4, +4, +4}, {+4, -4, +4},
      // Left
      {-4, -4, +4}, {-4, +4, +4}, {-4, +4, -4},
      {-4, -4, +4}, {-4, +4, -4}, {-4, -4, -4},
      // Top
      {-4, +4, +4}, {+4, +4, +4}, {+4, +4, -4},
      {-4, +4, +4}, {+4, +4, -4}, {-4, +4, -4},
      // Bottom
      {-4, -4, -4}, {+4, -4, -4}, {+4, -4, +4},
      {-4, -4, -4}, {+4, -4, +4}, {-4, -4, +4}}};
  // clang-format on

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void renderHorse(float angle, int increase_int);
  void createSkybox();
  void renderSkybox();
  void destroySkybox() const;
  void loadModel(std::string_view path);
};

#endif