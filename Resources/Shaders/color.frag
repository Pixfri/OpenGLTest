#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

uniform vec3 viewPos;

void main() {
    // Ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, UV));
    
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, UV));
    
    // Specular lighting    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, UV));
    
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}