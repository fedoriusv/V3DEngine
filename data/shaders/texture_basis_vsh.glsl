#version 330
#define POSITION    0
#define NORMAL      2
#define BINORMAL    3
#define TANGENT     4
#define TEXCOORD0   5

layout (location = POSITION)  in vec3 positions;
layout (location = NORMAL)    in vec3 normals;
layout (location = BINORMAL)  in vec3 binormals;
layout (location = TANGENT)   in vec3 tangents;
layout (location = TEXCOORD0) in vec2 texture0;

struct Transform
{
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat4 normalMatrix;
    vec3 viewPosition;
};
uniform Transform transform;

struct Light
{
    vec4    position;
    vec4    ambient;
    vec4    diffuse;
    vec4    specular;
};
uniform Light light[1];

struct Vertex 
{
    vec2 texCoord0;
    vec3 normal;
    vec3 binormal;
    vec3 tangent;
    
    vec3 viewDir;
    vec3 lightDir0;
};
out Vertex fragVertex;

void main()
{
    vec4 vertex = transform.modelMatrix * vec4(positions, 1.0);
    
    fragVertex.texCoord0 = texture0;
    fragVertex.lightDir0 = vec3(normalize(light[0].position - vertex));
    fragVertex.viewDir = vec3(normalize(vec4(transform.viewPosition, 1.0) - vertex));
    fragVertex.normal = vec3(normalize(transform.normalMatrix * vec4(normals, 0.0)));
    fragVertex.tangent  = vec3(normalize(transform.normalMatrix * vec4(tangents, 0.0)));
    fragVertex.binormal = vec3(normalize(transform.normalMatrix * vec4(binormals, 0.0)));

    gl_Position = transform.projectionMatrix * transform.viewMatrix * vertex;
}