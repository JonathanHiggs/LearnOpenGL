#version 330 core

in vec3 vFragPosition;
in vec3 vNormal;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;
uniform vec3 uLightPosition;
uniform vec3 uViewPosition;

out vec4 FragColor;

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(uLightPosition - vFragPosition);
    vec3 viewDirection = normalize(uViewPosition - vFragPosition);
    vec3 reflectedDirection = reflect(-lightDirection, vNormal);

    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * uLightColor;

    float diffuseStrength = 0.6f;
    vec3 diffuse = diffuseStrength * max(dot(normal, lightDirection), 0.0) * uLightColor;

    float specularStrength = 1.0f;
    vec3 specular = specularStrength * pow(max(dot(viewDirection, reflectedDirection), 0.0), 32) * uLightColor;

    vec3 result = (ambient + diffuse + specular) * uObjectColor;
    FragColor = vec4(result, 1.0);
}