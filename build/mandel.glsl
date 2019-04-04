#version 330 core

uniform int screenHeight;
uniform int screenWidth;

#define MAX_INTERATIONS 256

in vec4 gl_FragCoord;
out vec4 out_color;

void main(){
  // Screen coords => 0 to 1 range => -2 to 2 range
  vec2 c;
  c.x = ((gl_FragCoord.x/screenWidth) * 4.0) - 2.0;
  c.y = ((gl_FragCoord.y/screenHeight) * 4.0) - 2.0;

  vec2 z = vec2(0.0, 0.0);
  int i;

  for(i=0; i < MAX_INTERATIONS; i++){
    float x = (z.x * z.x - z.y * z.y) + c.x;
    float y = (z.x * z.y + z.y * z.x) + c.y;

    if((x * x + y * y) > 4.0){
      break;
    }
    z.x = x;
    z.y = y;
  }
  
  float color = i == MAX_INTERATIONS ? 0.0 : 1.0; //(float(i) / float(MAX_INTERATIONS)) * 1.0;
  out_color = vec4(vec3(color), 1.0);
}