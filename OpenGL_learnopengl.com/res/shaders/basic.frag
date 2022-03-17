#version 330 core
out vec4 FragColor;
in vec3 color;
uniform vec4 uniformVariable;

void main()
{
   FragColor = vec4(color + uniformVariable.xyz, 1.0);
}