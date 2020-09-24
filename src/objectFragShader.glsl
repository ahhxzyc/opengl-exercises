# version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 outColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    float ambientStrength = .1f;
    vec3 ambient = lightColor * ambientStrength;

    float diffuseRatio = max(dot(normal,lightDir), 0.f);
    vec3 diffuse = diffuseRatio * lightColor;

    float specularStrength = .5f;
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularRatio = pow(max(dot(reflectDir, viewDir), 0.f), 32);
    vec3 specular = specularStrength * specularRatio * lightColor;

    outColor = vec4((ambient + diffuse + specular) * objectColor, 1.f);
}