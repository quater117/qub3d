#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 mvp;

out vec3 pass_position;

void main()
{
    gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	pass_position = aPos;
}