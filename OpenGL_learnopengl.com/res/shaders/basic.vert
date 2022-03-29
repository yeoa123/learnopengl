#version 460 core

layout (location = 0) in vec2 vertexpos;
layout (location = 1) in vec3 incolor;
layout (location = 2) in vec2 intexcoords;
out vec3 color;
out vec2 texcoords;

void main()
{
   gl_Position = vec4(vertexpos.xy, 0.0, 1.0);
   color = incolor;
   texcoords = intexcoords;
};