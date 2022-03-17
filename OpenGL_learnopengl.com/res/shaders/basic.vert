#version 330 core

layout (location = 0) in vec3 vertexpos;
layout (location = 1) in vec3 incolor;
out vec3 color;

void main()
{
   gl_Position = vec4(vertexpos.xyz, 1.0);
   color = incolor;
};