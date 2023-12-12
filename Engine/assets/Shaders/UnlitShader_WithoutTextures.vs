 #version 410

in vec3 fragNormal;

out vec4 fragColor;

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0)); // Example light direction

    float diffuse = max(dot(normal, lightDirection), 0.0);

    fragColor = vec4(diffuse * vec3(1.0, 1.0, 1.0), 1.0); // White color for simplicity
}
