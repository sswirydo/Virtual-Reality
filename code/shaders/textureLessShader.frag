#version 330 core
out vec4 FragColor;

struct DirectionalLight {
    vec3 direction;
    vec4 lightColor;
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

struct SpotLight {
    vec3  position;
    vec3  direction;
    float cutOff;
    vec4 lightColor;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragPos;  
in vec2 TexCoords;
in float visibility;


#define NR_SPOTLIGHTS 30 
uniform Material material;
uniform DirectionalLight sun;
uniform SpotLight streetLight[NR_SPOTLIGHTS];

uniform vec4 lightColor;
uniform vec3 viewPos;   
uniform bool isNight;  

const vec4 fogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);


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

vec4 computeSpotLight(SpotLight streetLight, vec3 lightDirection, Material material, vec3 normal, vec3 viewDir){
    float theta = dot(lightDirection, normalize(-streetLight.direction));
    // ambient light
    vec4 ambient = vec4(material.ambient,material.transparency) * vec4(streetLight.ambient,1.0);
    if(theta > streetLight.cutOff) 
    {       

        // diffuse light 
        float diff = max(dot(normal, lightDirection), 0.0);
        vec4 diffuse = (diff * vec4(material.diffuse,material.transparency))* vec4(streetLight.diffuse,1.0);

        // specular light 
        vec3 reflectDir = reflect(-lightDirection, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec4 specular = (vec4(material.specular,material.transparency) * spec) * vec4(streetLight.specular,1.0);
        return (specular+ambient+diffuse);
    }
    else  // else, use ambient light so scene isn't completely dark outside the spotlight.
        return ambient/100.0;//vec4(0.0,0.0,0.0,0.4);
}

void main()
{    
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    // vec4 result = computeDirectionalLight(sun, material,normal, viewDir) * sun.lightColor;
    vec4 result = computeDirectionalLight(sun, material,normal, viewDir) * sun.lightColor;
    // vec4 result = vec4(0.0);

    for(int i = 0; i < NR_SPOTLIGHTS; i++){
        if(isNight){
            vec3 lightDir = normalize(streetLight[i].position - FragPos); // the vector pointing from the fragment to the light source.
            result += computeSpotLight(streetLight[i],lightDir,material,normal,viewDir)*streetLight[i].lightColor;
        }
    
    }
    result = mix(fogColor, result, visibility); // fog effect
    FragColor = result;
}
