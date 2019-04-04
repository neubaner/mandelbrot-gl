#version 330 core

uniform int screenHeight;
uniform int screenWidth;
uniform float zoom = 1.0;
uniform float offsetX = 0.0;
uniform float offsetY = 0.0;

#define MAX_INTERATIONS 256

in vec4 gl_FragCoord;
out vec4 out_color;

void main(){
  // Screen coords => 0 to 1 range => -2 to 2 range
  dvec2 c;
  
  c.x = (((gl_FragCoord.x/double(screenWidth)) * 4.0) - 2.0) * zoom + offsetX;
  c.y = (((gl_FragCoord.y/double(screenHeight)) * 4.0) - 2.0) * zoom + offsetY;

  dvec2 z = vec2(0.0, 0.0);
  int i;

  for(i=0; i < MAX_INTERATIONS; i++){
    double x = (z.x * z.x - z.y * z.y) + c.x;
    double y = (z.x * z.y + z.y * z.x) + c.y;

    if((x * x + y * y) > 4.0){
      break;
    }
    z.x = x;
    z.y = y;
  }
  
  double color = i == MAX_INTERATIONS ? 0.0 : (double(i) / double(MAX_INTERATIONS)) * 1.0;
  out_color = vec4(vec3(color), 1.0);
}