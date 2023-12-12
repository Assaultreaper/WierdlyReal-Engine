 #version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

out vec3 fragNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPosition = model * vec4(inPosition, 1.0);
    gl_Position = projection * view * worldPosition;

    fragNormal = mat3(transpose(inverse(model))) * inNormal;
}
