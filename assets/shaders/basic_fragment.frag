#version 330 core
out vec4 FragColor;

in vec3 pass_position;
in vec2 pass_uv;

float map(float value, float min1, float max1, float min2, float max2)
{
	float perc = (value - min1) / (max1 - min1);
	value = perc * (max2 - min2) + min2;
	return value;
}

uniform float colorMapRange;

uniform sampler2D texture1;

void main()
{
	float fmax = colorMapRange;
	
	vec3 col = pass_position;
	col.r = map(col.r, -1.0f, fmax, 0.0f, 1.0f);
	col.g = map(col.g, -1.0f, fmax, 0.0f, 1.0f);
	col.b = map(col.b, -1.0f, fmax, 0.0f, 1.0f);
	
    FragColor = vec4(col, 1.f) * texture(texture1, pass_uv);
} 
