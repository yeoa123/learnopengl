#version 330 core
out vec4 FragColor;
in vec4 pos;
uniform vec2 pulse;

void main()
{
   FragColor = vec4(0.0, (sin(16*3.1416*pos.x+pulse.x)+1)/2, (sin(4*3.1416*pos.y+pulse.y)+1)/2, 1.0);
}