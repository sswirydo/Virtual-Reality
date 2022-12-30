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
}; 

uniform Material material;
uniform DirectionalLight directionalLight;

  
in vec3 Normal;
in vec3 FragPos;  
in vec3 lightVector;
uniform vec4 lightColor;

uniform vec3 viewPos;   

vec4 computeDirectionalLight(DirectionalLight directionalLight, Material material, vec3 normal, vec3 viewDir){
    // ambient light
    vec4 ambient = vec4(material.ambient,1.0) * directionalLight.ambient;

    // diffuse light 
    vec3 lightDirection = normalize(-directionalLight.direction);  
    float diff = max(dot(normal, lightDirection), 0.0);
    vec4 diffuse = (diff * vec4(material.diffuse,1.0))* directionalLight.diffuse;

    // specular light 
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = (vec4(material.specular,1.0) * spec) * directionalLight.diffuse;
    return (specular+ambient+diffuse);
}


void main()
{    
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    FragColor = computeDirectionalLight(directionalLight, material,normal, viewDir) * lightColor ;
}
