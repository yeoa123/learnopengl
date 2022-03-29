#version 460 core
out vec4 FragColor;
in vec3 color;
in vec2 texcoords;
uniform sampler2D screen;

void main()
{
   FragColor = texture(screen, texcoords);
}