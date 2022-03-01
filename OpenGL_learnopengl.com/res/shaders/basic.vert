#version 330 core

layout (location = 0) in vec3 pos;
out vec4 vertexColor;

void main()
{
   gl_Position = vec4(pos.xyz, 1.0);
   vertexColor = vec4(0.3, 0.2, 0.6, 1.0);
};