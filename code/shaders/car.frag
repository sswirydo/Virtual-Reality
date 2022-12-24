#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;  
in vec3 lightVector;
uniform vec4 lightColor;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal;
uniform vec3 viewPos;   

void main()
{    
    // ambient light
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * lightColor;

    // diffuse light 
    vec3 normalizedNormal = normalize(Normal);
    vec3 lightDirection = normalize(lightVector);  
    float diff = max(dot(normalizedNormal, lightDirection), 0.0);
    vec4 diffuse = diff * lightColor;
    
    // specular light 
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, normalizedNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec4 specular = specularStrength * spec * lightColor;  

    FragColor = (texture(texture_diffuse1, TexCoords)) * (specular+ambient+diffuse);
}

