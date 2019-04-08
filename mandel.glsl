#version 400 core

uniform int screenHeight;
uniform int screenWidth;
uniform double zoom = 1.0;
uniform double offsetX = 0.0;
uniform double offsetY = 0.0;

#define MAX_INTERATIONS 256

in vec4 gl_FragCoord;
out vec4 out_color;

vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

void main(){
  // Screen coords => 0 to 1 range => -2 to 2 range
  dvec2 c;
  
  c.x = (((double(gl_FragCoord.x) / double(screenWidth)) * 4.0) - 2.0) * zoom + offsetX;
  c.y = (((double(gl_FragCoord.y) / double(screenHeight)) * 4.0) - 2.0) * zoom + offsetY;

  dvec2 z = dvec2(0.0, 0.0);
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
  
  out_color = i == MAX_INTERATIONS
            ? vec4(vec3(0.0), 1.0)
            : vec4(hsb2rgb(mix(vec3(1.0, 1.0, 1.0), vec3(0.0, 1.0, 1.0), vec3((double(i) / double(MAX_INTERATIONS)) * 1.0))), 1.0);
}