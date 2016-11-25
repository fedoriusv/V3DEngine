#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;

layout(std140, binding = 0) uniform Transform
{
    mat4 projectionMatrix;
    mat4 modelMatrix;
    mat4 viewMatrix;
} transform;

void main()
{
    vec4 vertex = transform.modelMatrix * vec4(inPosition, 1.0);
    gl_Position = transform.projectionMatrix * transform.viewMatrix * vertex;
}