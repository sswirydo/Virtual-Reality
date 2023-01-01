#version 330 core
out vec4 FragColor;
precision mediump float;
uniform samplerCube cubemapSampler;
in vec3 texCoord_v;

const vec4 fogColor = vec4(0.5f,0.5f,0.5f, 1.0f);

void main() {
	FragColor = texture(cubemapSampler,texCoord_v);

	// fog effect
	float visibility = texCoord_v.y;
	visibility = clamp(visibility, 0.0f, 1.0f);
	FragColor = mix(fogColor, FragColor, visibility);
}