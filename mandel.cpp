#include "gl.h"
#include "util.h"

#include <stdlib.h>

static const char* vertexShaderContent = 
"#version 330 core\n"
"layout (location = 0) in vec2 position;\n"
"\n"
"void main() {\n"
"  gl_Position = vec4(position, 0.0, 1.0);\n"
"}\n";

void setupRenderMandel(){
  int success;
  GLchar infoLog[512];

  GLfloat vertices[] = {
        -0.5f,  0.5f, // Top-left
         0.5f,  0.5f, // Top-right
         0.5f, -0.5f, // Bottom-right
        -0.5f, -0.5f,  // Bottom-left
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

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderContent, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      printf("Error compiling shader %s\n", infoLog);
  };

  char* fragShaderContents = readFile("mandel.glsl", NULL);
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, (const GLchar **)&fragShaderContents, NULL);
  glCompileShader(fragShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      printf("Error compiling shader %s\n", infoLog);
  };

  int program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);
  glUseProgram(program);


  free(fragShaderContents);
}

void renderMandel(){
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
