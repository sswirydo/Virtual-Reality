#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D atlas;
uniform bool enableColor;

void main()
{
	if (enableColor){
		color = vec4(1.0f, 0.72f, 0.72f, texture(atlas, TexCoords).r);
	}
	else {
		color = vec4(1.0f, 1.0f, 1.0f, texture(atlas, TexCoords).r);
	}
};