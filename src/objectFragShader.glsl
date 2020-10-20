# version 330 core

#define NUM_POINT_LIGHTS 2

struct PointLight {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 bTexCoords;

out vec4 outColor;

uniform float constant;
uniform float linear;
uniform float quadratic;

uniform PointLight lights[NUM_POINT_LIGHTS];

uniform sampler2D textureDiffuse0;
uniform sampler2D textureSpecular0;

vec3 calcPointLight(PointLight light, vec3 normal) {

    vec3 lightDir = normalize(light.pos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(textureDiffuse0, bTexCoords));
    // diffuse
    float diff = max(dot(normal, lightDir), 0.f);
    vec3 diffuse = diff * light.diffuse * vec3(texture(textureDiffuse0, bTexCoords));
    // specular
    float spec = pow(max(dot(reflectDir, lightDir), 0.f), 32.f);
    vec3 specular = light.specular * spec * vec3(texture(textureSpecular0, bTexCoords));
    // attenuation
    float dist = length(FragPos - light.pos);
    float attenuation = 1.f / (constant + linear * dist + quadratic * dist * dist);

    return attenuation * (ambient + diffuse + specular);
}

void main() {

    vec3 normal = normalize(Normal);

    vec3 colorSum = vec3(0.f);
    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        colorSum += calcPointLight(lights[i], normal);
    }
    
    outColor = vec4(colorSum, 1.f);
}