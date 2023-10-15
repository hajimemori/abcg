#include "window.hpp"

void Window::onCreate() {
  auto const *vertexShader{R"gl(#version 300 es
    layout(location = 0) in vec2 inPosition;

    void main() { 
      gl_PointSize = 2.0;
      gl_Position = vec4(inPosition, 0, 1); 
    }
  )gl"};

  auto const *fragmentShader{R"gl(#version 300 es
    precision mediump float;

    out vec4 outColor;

    void main() { outColor = vec4(1); }
  )gl"};

  // Create shader program
  m_program = abcg::createOpenGLProgram(
      {{.source = vertexShader, .stage = abcg::ShaderStage::Vertex},
       {.source = fragmentShader, .stage = abcg::ShaderStage::Fragment}});

  // Clear window
  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  std::array<GLfloat, 2> sizes{};
#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
  abcg::glGetFloatv(GL_POINT_SIZE_RANGE, sizes.data());
#else
  abcg::glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, sizes.data());
#endif
  fmt::print("Point size: {:.2f} (min), {:.2f} (max)\n", sizes.at(0),
             sizes.at(1));

  float meiaAresta = m_aresta/2;
  m_P = m_points.at(0);
  m_InicialPoints = {{{(-1 * meiaAresta), (-1 * meiaAresta)}, {meiaAresta, (-1 * meiaAresta)}, {meiaAresta, meiaAresta}, {(-1 * meiaAresta), meiaAresta}}};
  m_points = m_InicialPoints;
  m_atualPoint = 0;
}

void Window::onPaint() {
  if (m_timer.elapsed() < 1.0 / 20)
    return;
  m_timer.restart();

  if(m_atualPoint == 0){
    m_P = m_points.at(m_atualPoint);
    m_points.at(m_atualPoint) = {m_points.at(m_atualPoint).x + m_dif, m_points.at(m_atualPoint).y + m_dif};
    if(m_points.at(m_atualPoint).x > 0 || m_points.at(m_atualPoint).y > 0){
      m_points.at(m_atualPoint) = m_InicialPoints.at(m_atualPoint);
    }
    m_atualPoint++;
  }
  else if(m_atualPoint == 1){
    m_P = m_points.at(m_atualPoint);
    m_points.at(m_atualPoint) = {m_points.at(m_atualPoint).x - m_dif, m_points.at(m_atualPoint).y + m_dif};
    if(m_points.at(m_atualPoint).x < 0 || m_points.at(m_atualPoint).y > 0){
      m_points.at(m_atualPoint) = m_InicialPoints.at(m_atualPoint);
    }
    m_atualPoint++;
  }
  else if(m_atualPoint == 2){
    m_P = m_points.at(m_atualPoint);
    m_points.at(m_atualPoint) = {m_points.at(m_atualPoint).x - m_dif, m_points.at(m_atualPoint).y - m_dif};
    if(m_points.at(m_atualPoint).x < 0 || m_points.at(m_atualPoint).y < 0){
      m_points.at(m_atualPoint) = m_InicialPoints.at(m_atualPoint);
    }
    m_atualPoint++;
  }
  else{
    m_P = m_points.at(m_atualPoint);
    m_points.at(m_atualPoint) = {m_points.at(m_atualPoint).x + m_dif, m_points.at(m_atualPoint).y - m_dif};
    if(m_points.at(m_atualPoint).x > 0 || m_points.at(m_atualPoint).y < 0){
      m_points.at(m_atualPoint) = m_InicialPoints.at(m_atualPoint);
    }
    m_atualPoint = 0;
  }

  // Create OpenGL buffers for drawing the point at m_P
  setupModel();

  // Set the viewport
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // Start using the shader program
  abcg::glUseProgram(m_program);
  // Start using VAO
  abcg::glBindVertexArray(m_VAO);

  // Draw a single point
  abcg::glDrawArrays(GL_POINTS, 0, 1);

  // End using VAO
  abcg::glBindVertexArray(0);
  // End using the shader program
  abcg::glUseProgram(0);


  
  
  // Print coordinates to console
  fmt::print("({:+.2f}, {:+.2f})\n", m_P.x, m_P.y);
}

void Window::setupModel() {
  // Release previous VBO and VAO
  abcg::glDeleteBuffers(1, &m_VBOVertices);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Generate a new VBO and get the associated ID
  abcg::glGenBuffers(1, &m_VBOVertices);
  // Bind VBO in order to use it
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
  // Upload data to VBO
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_P), &m_P, GL_STATIC_DRAW);
  // Unbinding the VBO is allowed (data can be released now)
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  // Release shader program, VBO and VAO
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOVertices);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const widgetSize{ImVec2(330, 100)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5,
                                   m_viewportSize.y - widgetSize.y - 5));
    ImGui::SetNextWindowSize(widgetSize);
    auto const windowFlags{ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoCollapse |
                           ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin(" ", nullptr, windowFlags);

    ImGui::PushItemWidth(140);
    if(ImGui::SliderFloat("Distancia entre pontos", &m_dif, 0.01, 0.2 , "%f pixels")){
      abcg::glClear(GL_COLOR_BUFFER_BIT);
      m_points = m_InicialPoints;
    };
    ImGui::PopItemWidth();

    ImGui::PushItemWidth(140);
    if(ImGui::SliderFloat("Tamanho da aresta", &m_aresta, 0.3, 2 , "%f pixels")){
      abcg::glClear(GL_COLOR_BUFFER_BIT);
      float meiaAresta = m_aresta/2;
      m_InicialPoints = {{{(-1 * meiaAresta), (-1 * meiaAresta)}, {meiaAresta, (-1 * meiaAresta)}, {meiaAresta, meiaAresta}, {(-1 * meiaAresta), meiaAresta}}};
      m_points = m_InicialPoints;
    };
    ImGui::PopItemWidth();


    if (ImGui::Button("Clear window", ImVec2(-1, 30))) {
      abcg::glClear(GL_COLOR_BUFFER_BIT);
      m_points = m_InicialPoints;
    }

    ImGui::End();
  }
}