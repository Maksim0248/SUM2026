#version 330
 
layout(location = 0) out vec4 OutColor;
 
in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;
 
uniform float Time; 

void main( void )
{
  vec3 L;
  float nl;
  
  if (DrawPos.y > 3 * abs(sin(Time / 5)))
    discard;
  L = normalize(vec3(sin(Time), sin(Time), 1));
  
  nl = max(dot(DrawNormal, L), 0.4);
  OutColor = vec4(DrawNormal, 1) * nl;
  //DrawColor.rgb
}