#version 330 core

uniform float r;
uniform float g;
uniform float b;

out vec4 FragColor;

void main()
{
    FragColor = vec4(r, g, b, 1.0);
}