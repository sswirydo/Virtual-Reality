#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// out vec2 TexCoords;
out vec3 FragPos;  
out vec3 Normal;
out vec3 lightVector;
out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

// fog effect
const float density = 0.010f;
const float gradient = 1.6f;

void main()
{
    Normal = vec3(model * vec4(aNormal,0.0));
    vec4 worldPosition = model * vec4(aPos, 1.0);
    FragPos = worldPosition.xyz;

    vec4 positionRelativeToCam = view * worldPosition;
    float distance = length(positionRelativeToCam.xyz);
    visibility = exp(-pow(distance * density, gradient));
    visibility = clamp(visibility, 0.0, 1.0);

    gl_Position = projection * positionRelativeToCam;
    lightVector = lightPos - worldPosition.xyz;

    
}