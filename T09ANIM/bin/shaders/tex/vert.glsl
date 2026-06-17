/**/

#version 460
 
// кто куда приходит
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;
 
// глобальные переменные (произведение матриц: World * View * Proj)
uniform mat4 MatrWVP;

 
// выходные параметры (varying)
out vec4 DrawColor;
out vec2 DrawTexCoord;
 
void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawColor = InColor;
  DrawTexCoord = InTexCoord;
}