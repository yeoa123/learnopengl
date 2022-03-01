#version 330 core

layout (location = 0) in vec3 vertexpos;
out vec4 pos;

void main()
{
   gl_Position = vec4(vertexpos.xyz, 1.0);
   pos = vec4(vertexpos.xyz, 1.0);
};