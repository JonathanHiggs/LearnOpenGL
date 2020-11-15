#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
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

#define NUMBER_POINT_LIGHTS 4
#define NUMBER_SPOT_LIGHTS 1

in vec3 vFragPosition;
in vec3 vNormal;
in vec2 vTexCoords;

uniform vec3 uViewPosition;
uniform Material uMaterial;
uniform DirectionalLight uDirectionalLight;
uniform PointLight uPointLights[NUMBER_POINT_LIGHTS];
uniform SpotLight uSpotLights[NUMBER_SPOT_LIGHTS];

out vec4 FragColor;


vec3 directionalLightColor(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 pointLightColor(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection);
vec3 spotLightColor(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection);


void main()
{
    vec3 normal = normalize(vNormal);
    vec3 viewDirection = normalize(uViewPosition - vFragPosition);

    vec3 color = directionalLightColor(uDirectionalLight, normal, viewDirection);

    for (int i = 0; i < NUMBER_POINT_LIGHTS; i++)
        color += pointLightColor(uPointLights[i], normal, vFragPosition, viewDirection);

    for (int i = 0; i < NUMBER_SPOT_LIGHTS; i++)
        color += spotLightColor(uSpotLights[i], normal, vFragPosition, viewDirection);

    FragColor = vec4(color, 1.0f);
}

vec3 directionalLightColor(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-light.direction);
    vec3 reflectedDirection = reflect(-lightDirection, normal);

    float diff = max(dot(normal, lightDirection), 0.0f);
    float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0f), uMaterial.shininess);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.diffuse, vTexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(uMaterial.diffuse, vTexCoords));
    vec3 specular = light.specular * spec * vec3(texture(uMaterial.specular, vTexCoords));

    return (ambient + diffuse + specular);
}

vec3 pointLightColor(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - fragPosition);
    vec3 reflectedDirection = reflect(-lightDirection, normal);

    float diff = max(dot(normal, lightDirection), 0.0f);
    float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0f), uMaterial.shininess);
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.diffuse, vTexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(uMaterial.diffuse, vTexCoords));
    vec3 specular = light.specular * spec * vec3(texture(uMaterial.specular, vTexCoords));

    return (ambient + diffuse + specular) * attenuation;
}

vec3 spotLightColor(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - fragPosition);
    vec3 reflectedDirection = reflect(-lightDirection, normal);

    float diff = max(dot(normal, lightDirection), 0.0);
    float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0f), uMaterial.shininess);
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.diffuse, vTexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(uMaterial.diffuse, vTexCoords));
    vec3 specular = light.specular * spec * vec3(texture(uMaterial.specular, vTexCoords));

    return (ambient + diffuse + specular) * attenuation * intensity;
}