# Carrossel

## Grupo

* 11043111 - Clayton de Oliveira
* 21045116 - Rafael Hajime Mori

## Descrição 

Este projeto foi baseado na atividade anterior: Carrossel. Incrementando a ideia utilizando texturas.

***
## Renderização dos Cavalos 

No método abaixo é utilizado para carregar o modelo:
```
void Window::loadModel(std::string_view path) {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  m_model.destroy();

  m_model.loadDiffuseTexture(assetsPath + "maps/oak_wood.jpg");
  m_model.loadNormalTexture(assetsPath + "maps/oak_wood.jpg");
  m_model.loadCubeTexture(assetsPath + "maps/cube/");
  m_model.loadObj(path);
  m_model.setupVAO(m_program);
  // Use material properties from the loaded model
  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();
}
```
Note que neste ponto é onde mostra que estamos utilizando *Textura Difusa*. 


E já no método abaixo é utilizado para renderização e posição do calvalo 
```
void Window::renderHorse(float m_angle, int increase_int) {

  // Draw horse
  float new_angle = m_angle + glm::radians(increase_int*m_angle_increase) + (m_rotateSpeed / 100);
  float x  = m_raio * cos(new_angle);
  float z  = m_raio * sin(new_angle);
  m_modelMatrix = glm::mat4(1.0);
  m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(x, m_height, z));
  m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-90.0f), glm::vec3(1, 0, 0));
  m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-90.0f), glm::vec3(0, 0, 1));
  m_modelMatrix = glm::rotate(m_modelMatrix, atan2f(x,z), glm::vec3(0, 0, 1));
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(0.3f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &m_modelMatrix[0][0]);
  m_model.render();
}
```



## Skybox 

O método abaixo é o método utilizado para criar o cubo que sera utilizado como skybox que simula o ambiente de campo aberto: 
```
void Window::createSkybox() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Create skybox program
  auto const path{assetsPath + "shaders/skybox"};
  m_skyProgram = abcg::createOpenGLProgram(
      {{.source = path + ".vert", .stage = abcg::ShaderStage::Vertex},
       {.source = path + ".frag", .stage = abcg::ShaderStage::Fragment}});
  // Generate VBO
  abcg::glGenBuffers(1, &m_skyVBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyPositions), m_skyPositions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  // Get location of attributes in the program
  auto const positionAttribute{abcg::glGetAttribLocation(m_skyProgram, "inPosition")};
  // Create VAO
  abcg::glGenVertexArrays(1, &m_skyVAO);
  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_skyVAO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}
``` 
E o método seguinte é utilizado para a renderização da skybox acima: 
```
void Window::renderSkybox() {
  abcg::glUseProgram(m_skyProgram);

  auto const viewMatrixLoc{abcg::glGetUniformLocation(m_skyProgram, "viewMatrix")};
  auto const projMatrixLoc{abcg::glGetUniformLocation(m_skyProgram, "projMatrix")};
  auto const skyTexLoc{abcg::glGetUniformLocation(m_skyProgram, "skyTex")};
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.getProjMatrix()[0][0]);
  abcg::glUniform1i(skyTexLoc, 0);
  abcg::glBindVertexArray(m_skyVAO);
  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_CUBE_MAP, m_model.getCubeTexture());
  abcg::glEnable(GL_CULL_FACE);
  abcg::glFrontFace(GL_CW);
  abcg::glDepthFunc(GL_LEQUAL);
  abcg::glDrawArrays(GL_TRIANGLES, 0, m_skyPositions.size());
abcg::glDepthFunc(GL_LESS);
```




