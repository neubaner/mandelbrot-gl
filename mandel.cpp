#include "gl.h"
#include "util.h"

#include <stdlib.h>

static const char* vertexShaderContent = 
"#version 150 core"
"in vec2 position;"
""
"void main() {"
"  gl_Position = vec4(position, 0.0, 1.0);"
"}";

void setupRenderMandel(){
  static float vertices[] = {
    -1.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, -1.0f,
    -1.0f, -1.0f
  };
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  static GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
  };
  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  int program = glCreateProgram();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderContent, NULL);
  glCompileShader(vertexShader);
  glAttachShader(program, vertexShader);

  char* fragShaderContents = readFile("mandel.glsl", NULL);
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fragShaderContents, NULL);
  glCompileShader(fragShader);
  glAttachShader(program, fragShader);

  glBindFragDataLocation(program, 0, "outColor");
  glLinkProgram(program);
  glUseProgram(program);

  GLint positionAttr = glGetAttribLocation(program, "position");
  glVertexAttribPointer(positionAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(positionAttr);

  free(fragShaderContents);
}

void renderMandel(){
  glDrawElements(GL_TRIANGLES, )
}