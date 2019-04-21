#include <cg_window.hpp>
#include <stdlib.h>

namespace cgicmc {

Window::Window() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //primeiro arg: o que configurar, segundo arg: val da configuracao
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Window::~Window() { glfwTerminate(); }

void Window::framebufferCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height); //[x, x+largura]x[y, y+altura]
}

GLFWwindow* Window::getwindow(){ // Função criada para retornar a janela.
	return this->_window; 
}

void Window::createWindow() {
  _window = glfwCreateWindow(500, 500, "CATAVENTO - UNICORN 2019", NULL, NULL); //largura, altura, titulo da janela, monitor (?), share (?)
  if (_window == NULL) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(_window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //carrega asfuncoes da OpenGL
    std::cout << "Failed to initialize GLAD\n";
    exit(-2);
  }

  glfwSetFramebufferSizeCallback(_window, this->framebufferCallback);
}

void Window::processInput() {
  if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //detceta as teclas presionadas
    glfwSetWindowShouldClose(_window, true);
}



void Window::run() {
  while (!glfwWindowShouldClose(_window)) {

    // Comandos de entrada
    processInput();

    // Comandos de renderizacao vao aqui
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); //limpa o buffer para nao deixar resquicios do frame anterior
    // etc...

    // Controla eventos e troca os buffers para renderizacao
    glfwSwapBuffers(_window);
    glfwPollEvents();
  }
}
} // namespace cgicmc