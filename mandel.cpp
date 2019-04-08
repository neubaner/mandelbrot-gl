#include <stdlib.h>
#include <assert.h>

#include "gl.h"
#include "util.h"
#include "mandel.h"

enum MandelKey {
  MandelKeyUp,
  MandelKeyDown,
  MandelKeyLeft,
  MandelKeyRight,
  MandelKeyZoomIn,
  MandelKeyZoomOut,
};

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
  double offsetX;
  double offsetY;
  double zoom;
};

RenderMandelData setupRenderMandel(){
  int success;
  GLchar infoLog[512];

  // Setup buffers
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLfloat vertices[] = {
    -1.0f,  1.0f, // Top-left
     1.0f,  1.0f, // Top-right
     1.0f, -1.0f, // Bottom-right
    -1.0f, -1.0f,  // Bottom-left
  };
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint elements[] = {
    0, 1, 2,
    2, 3, 0,
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
  glBindFragDataLocation(program, 0, "out_color");
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if(!success) {
      glGetProgramInfoLog(program, 512, NULL, infoLog);
      printf("Error linking program %s", infoLog);
  }

  int positionLoc = glGetAttribLocation(program, "position");

  glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(positionLoc);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  free(fragShaderContents);

  return { program, vao };
}

void mandelHandleInput(RenderMandelData* data, MandelKey key){
  switch(key){
    case MandelKeyUp:
      data->offsetY += 0.1 * data->zoom;
      break;
    case MandelKeyDown:
      data->offsetY -= 0.1 * data->zoom;
      break;
    case MandelKeyLeft:
      data->offsetX -= 0.1 * data->zoom;
      break;
    case MandelKeyRight:
      data->offsetY += 0.1 * data->zoom;
      break;
    case MandelKeyZoomIn:
      data->zoom /= 1.1;
      break;
    case MandelKeyZoomOut:
      data->zoom *= 1.1;
      break;
    default:
      assert(true);
  }
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
  glUniform1d(offsetXLoc, offsetX);
  glUniform1d(offsetYLoc, offsetY);
  glUniform1d(zoomLoc, zoom);

  glBindVertexArray(data.vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const GLvoid *)0);
  glBindVertexArray(0);
}
