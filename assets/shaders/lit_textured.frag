#version 330 core

in vec2 vTexCoord;
in vec3 vWorldNormal;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec4 uColor;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;
uniform float uLightIntensity;
uniform vec3 uAmbientColor;
uniform float uAmbientIntensity;

void main()
{
    vec3 normal = normalize(vWorldNormal);
    vec3 lightDir = normalize(-uLightDirection);

    float lambert = max(dot(normal, lightDir), 0.0);

    vec3 ambient = uAmbientColor * uAmbientIntensity;
    vec3 diffuse = uLightColor * uLightIntensity * lambert;

    vec4 baseColor = texture(uTexture, vTexCoord) * uColor;
    FragColor = vec4(baseColor.rgb * (ambient + diffuse), baseColor.a);
}