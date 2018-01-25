#version 150

in vec2 vPosition;
in vec4 cPosition;
out vec4 color;

void main() 
{ 
  gl_Position = vec4(vPosition.x, vPosition.y, 0, 1);
  color = cPosition;
}