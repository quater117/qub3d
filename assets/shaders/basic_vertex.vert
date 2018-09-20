#version 330 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 mvp;

out vec3 pass_position;
out vec2 pass_uv;

void main()
{
    gl_Position = mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	pass_position = aPos;
	pass_uv = aUV;
}
