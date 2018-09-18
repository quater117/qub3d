#version 330 core
out vec4 FragColor;

in vec3 pass_position;

void main()
{
    FragColor = vec4(pass_position, 1.0f);
} 