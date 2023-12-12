#version 330 core

in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D textureSampler;

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0)); // Example light direction

    float diffuse = max(dot(normal, lightDirection), 0.0);

    vec4 textureColor = texture(textureSampler, fragTexCoord);
    fragColor = vec4(diffuse * textureColor.rgb, textureColor.a);
}
