# Carrossel

Este projeto foi baseado na atividade LookAt e tem como principal objetivo a simulação de um brinquedo infantil conhecido como Carrossel.
O brinquedo consiste repetidos modelos que giram em circulos por um circuito sobe e desce de forma uniforme até uma altura máxima.

***
## Alteração dinamica de variáveis

Criamos alterações para que algumas variáveis sejam possíveis de serem alteradas conforme a vontade do usuário. Para isso utilizamos um Slider na função onPaintUI() e a função ficou da seguinte forma:
```
void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();
  {
    auto const widgetSize{ImVec2(330, 120)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5,
                                   m_viewportSize.y - widgetSize.y - 5));
    ImGui::SetNextWindowSize(widgetSize);
    auto const windowFlags{ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoCollapse |
                           ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin(" ", nullptr, windowFlags);

    ImGui::PushItemWidth(140);
    ImGui::SliderFloat("Velocidade de subida", &m_uppingScale, 0.1, 1.0 , "%f" );
    ImGui::PopItemWidth();

    ImGui::PushItemWidth(140);
    ImGui::SliderFloat("Altura maxima", &m_maxHeight, 0.0, 1.0 , "%f" );
    ImGui::PopItemWidth();

    ImGui::PushItemWidth(140);
    ImGui::SliderFloat("Velocidade de rotacao", &m_rotateSpeed, 0.01, 0.10 , "%f" );
    ImGui::PopItemWidth();

    ImGui::PushItemWidth(140);
    ImGui::SliderFloat("Raio de rotacao", &m_raio, 0.1, 2.0 , "%f" );
    ImGui::PopItemWidth();

    ImGui::End();
  }
 }
```

Neste cógido temos alguns Sliders. O primeiro é o seguinte:
```
    ImGui::PushItemWidth(140);
    ImGui::SliderFloat("Velocidade de subida", &m_uppingScale, 0.1, 1.0 , "%f" );
    ImGui::PopItemWidth();
```
E nesse Slide controlamos a variável m_uppingScale que é utilizada para controlar a velocida do movimento vertical realizado pelo modelo. Podemos notar tambem, que para fins de facilitar o valor desta variável para o usuário, em todos os momentos em que é utilizado esta variável dividimos o valor por 1000.

Em seguida temos o seguinte Slider:
```
    ImGui::PushItemWidth(140);
    ImGui::SliderFloat("Altura maxima", &m_maxHeight, 0.0, 1.0 , "%f" );
    ImGui::PopItemWidth();
```
No qual podemos controlar a altura máxima da movimentação vertical do modelo.

O proximo Slider é utilizado para controlar a velocidade de rotação horizontal do modelo. Podemos notar que, assim como a variável de velocidade vertical do modelo, a fim melhorar a usabilidade do usuário, o seu valor tambem é divido por 100 antes de sua utilização e seu código esta descrito como:

```
    ImGui::PushItemWidth(140);
    ImGui::SliderFloat("Velocidade de rotacao", &m_rotateSpeed, 0.01, 0.10 , "%f" );
    ImGui::PopItemWidth();
```

E por fim temos o Slider que controla a variável de raio de rotação do modelo que esta descrito como:

```
    ImGui::PushItemWidth(140);
    ImGui::SliderFloat("Raio de rotacao", &m_raio, 0.1, 2.0 , "%f" );
    ImGui::PopItemWidth();
```



## Movimentação circular do modelo

Para realizar a movimentação do modelo utilizamos uma função baseado em seno e cosseno para encontrar a posição de x e y com base no angulo em que o modelo está:

* x = raio * cos(angulo)
* y = raio * seno(angulo)

Para cada modelo desenhado na cena o angulo inicial é acrescido em 60˚, como são 6 modelos temos os 360˚ preenchidos.

Para isto, em código, alteramos na função onPaint() para realizar as equações acima e gravar em variaveis que serão passadas como parametro na função glm::translate:

```
  float x = m_raio * cos(m_angle);
  float z = m_raio * sin(m_angle);

  .
  .
  .

  model = glm::translate(model, glm::vec3(x, m_height, z));
  ```

## Movimentação vertical do modelo

Para realização do movimento vertical do modelo, inicialmente checamos se a altura do modelo alcançou o valor máximo (utilizando a valor selecionado pelo usuário) ou o valor mínimo (zero), e em seguida caso o modelo esteja subindo somamos o valor da variavel de velocidade vertical do modelo na variável que guarda o valor da altura. E caso o modelo nao esteja subido, subtraimos este valor. E em seguida passamos esse valor na função glm::translate, para atribuir a altura no vec3. Utilizando o seguinte código:

```
if(m_height > m_maxHeight){
    m_upping = false;
  }
  if(m_height <= 0){
    m_upping = true;
  }

  if(m_upping){
    m_height += (m_uppingScale / 1000);
  }
  else{
    m_height -= (m_uppingScale / 1000);
  }

  model = glm::translate(model, glm::vec3(x, m_height, z));

```

## Rotação em torno do proprio eixo do modelo

Para dar a impressão de um carrossel, devemos fazer uma rotação do modelo em torno do proprio eixo para que ele sempre esteja apontando para frente enquanto realiza a rotação do carrossel. Para isso utilizamos a função de arco tangente já existente no c++, e passamos como parametro da funçao glm::rotate do modelo utilizando o código abaixo:

```
model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 0, 1));
model = glm::rotate(model, atan2f(x,z), glm::vec3(0, 0, 1));
```

## Desenho do chão

Para dar um efeito de base para o carrossel, o chão é colorido de cinza com o tamanho suficiente para comportar o raio de rotacao, a demais área foi colorida de verde para um efeito de grama.

```
if (z < -m_raio || z > m_raio || x > m_raio || x < - m_raio){
  abcg::glUniform4f(m_colorLoc,  0.047, 0.502, 0.082, 1.0f); // Verde
}else{
  abcg::glUniform4f(m_colorLoc, 0.5f, 0.5f, 0.5f, 1.0f); // Cinza
}
```


## Grupo

11043111 - Clayton de Oliveira
21045116 - Rafael Hajime Mori