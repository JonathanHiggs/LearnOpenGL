#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

    vec3 ambient = uLight.ambient * uMaterial.ambient;

    float diffuseIntensity = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = uLight.diffuse * (diffuseIntensity * uMaterial.diffuse);

    float specularIntensity = pow(max(dot(viewDirection, reflectedDirection), 0.0), uMaterial.shininess);
    vec3 specular = uLight.specular * (specularIntensity * uMaterial.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}