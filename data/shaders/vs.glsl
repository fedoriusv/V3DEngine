#version 330
#define POSITION 	0
#define NORMAL 		2

layout (location = POSITION) in vec3 positions;
layout (location = NORMAL)   in vec3 normals;

struct Transform
{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 normalMatrix;
	vec3 viewPosition;
};
uniform Transform transform;

struct Vertex 
{
	vec3  normal;
	vec3  view;
};
out Vertex vertex;

void main()
{
    vec4 position = transform.modelMatrix * vec4(positions, 1.0);
	vertex.normal = (transform.normalMatrix * vec4(normals, 0.0)).xyz;
	vertex.view   = normalize(transform.viewPosition - vec3(position));
	
    gl_Position   = transform.viewMatrix * position;
}