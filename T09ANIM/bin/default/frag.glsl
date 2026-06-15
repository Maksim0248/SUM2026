#version 330
 
// выходные параметры - цвет рисования
layout(location = 0) out vec4 OutColor;
 
// входные параметры (varying)
in vec4 DrawColor;
 
void main( void )
{
  OutColor = DrawColor;
}