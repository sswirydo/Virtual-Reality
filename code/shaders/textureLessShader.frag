#version 330 core
out vec4 FragColor;

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float transparency;
}; 

in vec3 Normal;
in vec3 FragPos;  
in vec2 TexCoords;

uniform Material material;
uniform DirectionalLight sun;
uniform vec4 lightColor;
uniform vec3 viewPos;   

vec4 computeDirectionalLight(DirectionalLight sun, Material material, vec3 normal, vec3 viewDir){
    // ambient light
    vec4 ambient = vec4(material.ambient,material.transparency) * vec4(sun.ambient,1.0);

    // diffuse light 
    vec3 lightDirection = normalize(-sun.direction);  
    float diff = max(dot(normal, lightDirection), 0.0);
    vec4 diffuse = (diff * vec4(material.diffuse,material.transparency))* vec4(sun.diffuse,1.0);

    // specular light 
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = (vec4(material.specular,material.transparency) * spec) * vec4(sun.specular,1.0);
    return (specular+ambient+diffuse);
}


void main()
{    
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec4 result = (computeDirectionalLight(sun, material,normal, viewDir)) * lightColor;
    FragColor = result;
}
