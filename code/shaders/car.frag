#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;  
in vec3 lightVector;

uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_shininess;
uniform vec3 material_transparency;


uniform vec4 lightColor;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal;
uniform vec3 viewPos;   

void main()
{    
    // ambient light
    float transparency = 0.4;
    vec4 ambient = vec4(material_ambient,transparency) * lightColor ;

    // diffuse light 
    vec3 normalizedNormal = normalize(Normal);
    vec3 lightDirection = normalize(lightVector);  
    float diff = max(dot(normalizedNormal, lightDirection), 0.0);
    vec4 diffuse = (diff*vec4(material_diffuse,transparency)) * lightColor;
    
    // specular light 
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, normalizedNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    vec4 specular = (vec4(material_specular,transparency) * spec) * lightColor;  

    FragColor = (texture(texture_diffuse1, TexCoords)) * (specular+ambient+diffuse);
}

