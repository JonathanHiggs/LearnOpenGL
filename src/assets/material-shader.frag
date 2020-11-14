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

in vec3 vFragPosition;
in vec3 vNormal;
in vec2 vTexCoords;

uniform vec3 uViewPosition;
uniform Material uMaterial;
uniform Light uLight;

out vec4 FragColor;

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(uLight.position - vFragPosition);
    vec3 viewDirection = normalize(uViewPosition - vFragPosition);
    vec3 reflectedDirection = reflect(-lightDirection, vNormal);

    vec3 ambient = uLight.ambient * vec3(texture(uMaterial.diffuse, vTexCoords));

    float diffuseIntensity = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = uLight.diffuse * diffuseIntensity * vec3(texture(uMaterial.diffuse, vTexCoords));

    float specularIntensity = pow(max(dot(viewDirection, reflectedDirection), 0.0), uMaterial.shininess);
    vec3 specular = uLight.specular * specularIntensity * vec3(texture(uMaterial.specular, vTexCoords));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}