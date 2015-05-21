#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <fstream>


using namespace std;

string readShader(const char* inFile)
{
  // compile vertex shader
  ifstream in (inFile);
  if (in.is_open())
  {
    string contents((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();
    return contents;
  }

  return NULL;
}

void error_callback(int error, const char* description)
{
  cerr << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

int main() {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
  {
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

  GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
  if (!window)
  {
    cerr << "Failed to create window." << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glewExperimental=true;
  if (glewInit() != GLEW_OK)
  {
    cerr << "Failed to initialize GLEW." << endl;
    exit(EXIT_FAILURE);
  }

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GLfloat g_vertex_buffer_data[] = {
    0.0f, 0.5f,
    0.5f, -0.5f,
    -0.5f, -0.5f
  };

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  // compile vertex shader
  string vSourceS = readShader("vert.glsl");
  const char* vertexSource = vSourceS.c_str();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  // check shader compile
  GLint status;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE)
  {
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    cerr << "Error in shader file vert.glsl" << endl;
    cerr << buffer << endl;
    exit(EXIT_FAILURE);
  }

  // compile fragment shader
  string fSourceS = readShader("frag.glsl");
  const char* fragmentSource = fSourceS.c_str();

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  //check fragment shader compile
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE)
  {
    char buffer[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
    cerr << "Error in shader file frag.glsl" << endl;
    cerr << buffer << endl;
    cerr << fragmentSource << endl;
    exit(EXIT_FAILURE);
  }

  // build shaders
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glBindFragDataLocation(shaderProgram, 0, "outColor");

  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posAttrib);

  glfwSetKeyCallback(window, key_callback);
  glfwSwapInterval(1);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  while (!glfwWindowShouldClose(window))
  {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
      0,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      (void*)0
    );

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  exit(EXIT_SUCCESS);
}
