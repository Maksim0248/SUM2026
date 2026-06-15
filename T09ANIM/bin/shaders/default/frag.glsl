#version 330
 
layout(location = 0) out vec4 OutColor;
 
in vec4 DrawColor;
in vec3 N;
 
void main( void )
{
  vec3 L;
  float nl;

  L = normalize(vec3(1, 1, 1));
  
  nl = dot(N, L);
  if (nl < 0.4)
    nl = 0.4;
  OutColor = DrawColor * nl;
}