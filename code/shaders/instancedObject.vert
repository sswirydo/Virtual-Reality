#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aOffset;

//out vec2 TexCoords;
out vec3 FragPos;  
out vec3 Normal;
out vec3 lightVector;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;  

void main()
{
    Normal = vec3(model * vec4(aNormal,0.0));
    vec4 worldPosition = model * vec4(aPos + aOffset, 1.0);
    FragPos = worldPosition.xyz;
    gl_Position = projection * view * worldPosition;
    lightVector = lightPos - worldPosition.xyz;
}