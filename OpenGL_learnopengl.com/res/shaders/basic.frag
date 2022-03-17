#version 330 core
out vec4 FragColor;
in vec4 pos;
uniform vec4 uniformVariable;

void main()
{
   FragColor = vec4(0.0, (sin(16*3.1416*pos.x+uniformVariable.x)+1)/2, (sin(4*3.1416*pos.y+uniformVariable.y)+1)/2, 1.0);
}