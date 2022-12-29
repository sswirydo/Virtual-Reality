#version 330 core
out vec4 FragColor;


uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_shininess;

  
in vec3 Normal;
//in vec2 TexCoords;
in vec3 FragPos;  
in vec3 lightVector;
// uniform vec4 lightColor;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal;
uniform vec3 viewPos;   

void main()
{    

    vec4 lightColor = vec4(1.0,1.0,1.0,1.0);
    // ambient light
    vec4 ambient = vec4(material_ambient,1.0);// * lightColor;

    // diffuse light 
    vec3 normalizedNormal = normalize(Normal);
    vec3 lightDirection = normalize(lightVector);  
    float diff = max(dot(normalizedNormal, lightDirection), 0.0);
    vec4 diffuse = (diff * vec4(material_diffuse,1.0));//* lightColor ;
    
    // specular light 
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, normalizedNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    vec4 specular = (vec4(material_specular,1.0) * spec); //* lightColor;  

    FragColor = (specular+ambient+diffuse);

    FragColor = vec4(material_diffuse, 1.0); // TODO: TEMPORARY
}
