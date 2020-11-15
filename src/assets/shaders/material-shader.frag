#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
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
    //vec3 lightDirection = normalize(-uLight.direction);
    vec3 viewDirection = normalize(uViewPosition - vFragPosition);
    vec3 reflectedDirection = reflect(-lightDirection, vNormal);

    float distance = length(uLight.position - vFragPosition);
    float attenuation =
        1.0 / (uLight.constant
                + uLight.linear * distance
                + uLight.quadratic * distance * distance);

    float theta = dot(lightDirection, normalize(-uLight.direction));
    float epsilon = uLight.cutOff - uLight.outerCutOff;
    float intensity = clamp((theta - uLight.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient =
        uLight.ambient
        * vec3(texture(uMaterial.diffuse, vTexCoords))
        * attenuation;

    float diffuseIntensity =
        max(dot(normal, lightDirection), 0.0)
        * attenuation
        * intensity;

    vec3 diffuse = uLight.diffuse * diffuseIntensity * vec3(texture(uMaterial.diffuse, vTexCoords));

    float specularIntensity =
        pow(max(dot(viewDirection, reflectedDirection), 0.0), uMaterial.shininess)
        * attenuation
        * intensity;

    vec3 specular = uLight.specular * specularIntensity * vec3(texture(uMaterial.specular, vTexCoords));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}