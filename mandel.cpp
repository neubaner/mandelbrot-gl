#include "gl.h"
#include "util.h"

#include <stdlib.h>

static const char* vertexShaderContent = 
"#version 330 core\n"
"in vec2 position;\n"
"\n"
"void main() {\n"
"  gl_Position = vec4(position, 0.0, 1.0);\n"
"}\n";

struct RenderMandelData {
  int program;
  GLuint vao;
};

RenderMandelData setupRenderMandel(){
  int success;
  GLchar infoLog[512];

  // Setup buffers
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLfloat vertices[] = {
     1.0f,  1.0f, // Top-right
     1.0f, -1.0f, // Bottom-right
    -1.0f, -1.0f,  // Bottom-left
    -1.0f,  1.0f, // Top-left
  };
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint elements[] = {
    0, 1, 3,
    1, 2, 3,
  };
  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  // Create Shaders

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderContent, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      printf("Error compiling shader %s\n", infoLog);
  };

  char* fragShaderContents = readFile("mandel.glsl", NULL);
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, (const GLchar **)&fragShaderContents, NULL);
  glCompileShader(fragShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      printf("Error compiling shader %s\n", infoLog);
  };

  // Link program

  int program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if(!success) {
      glGetProgramInfoLog(program, 512, NULL, infoLog);
      printf("Error linking program %s", infoLog);
  }

  int positionLoc = glGetAttribLocation(program, "position");

  glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(positionLoc);

  free(fragShaderContents);

  return { program, vao };
}

void renderMandel(RenderMandelData data, int screenWidth, int screenHeight, float offsetX, float offsetY, float zoom){
  glUseProgram(data.program);

  int screenWidthLoc = glGetUniformLocation(data.program, "screenWidth");
  int screenHeightLoc = glGetUniformLocation(data.program, "screenHeight");
  int offsetXLoc = glGetUniformLocation(data.program, "offsetX");
  int offsetYLoc = glGetUniformLocation(data.program, "offsetY");
  int zoomLoc = glGetUniformLocation(data.program, "zoom");

  glUniform1i(screenWidthLoc, screenWidth);
  glUniform1i(screenHeightLoc, screenHeight);
  glUniform1f(offsetXLoc, offsetX);
  glUniform1f(offsetYLoc, offsetY);
  glUniform1f(zoomLoc, zoom);

  glBindVertexArray(data.vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid *)0);
  glBindVertexArray(0);
}
