/**/

#version 460
 
// выходные параметры - цвет рисования
layout(location = 0) out vec4 OutColor;
 
// входные параметры (varying)
in vec4 DrawColor;
in vec2 DrawTexCoord;
 
layout(binding = 2) uniform sampler2D Tex;

void main( void )
{
  vec4 tc = texture(Tex, DrawTexCoord);
  OutColor = vec4(tc.rgb, 0);
}