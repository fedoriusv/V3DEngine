V3DEngine
===============

Next level after GLliba
Small modified Valera Engine

Render Technique

RenderTechnique structure represent in xml format:

technique tag:
name - name of render technique;
version - version of file

Example:
<technique name="techniqueName" version="100">...</technique>

...

pass tag:
name - [required] name of render pass

Example:
 <pass name="pass0">...</pass>
 
included tags:
<uniforms></uniforms>
...

uniforms tag:
included var tags:

var tag:
name - [required] name of uniform. Must be same just in shader.
type - [required/optional] type of data of uniform. Must be same just in shader. If uniform has fixed value, then this value ignored.
Used next fixed values:
	"int"
    "float"
    "double"
    "vec2"
    "vec3"
    "vec4"
    "mat3"
    "mat4"
	
val - [required/optional] initial value of uniform. If used user data, then need to specify type of data value  
Used next fixed values:
	"transform.projectionMatrix"
    "transform.modelMatrix"
    "transform.viewMatrix"
    "transform.normalMatrix"
    "transform.viewPosition"
    "transform.viewUpVector"
    "material.ambient"
    "material.diffuse"
    "material.specular"
    "material.emission"
    "material.shininess"
    "material.transparency"
    "lights.count"
    "light.position"
    "light.ambient"
    "light.diffuse"
    "light.specular"
    "light.direction"
    "light.attenuation"
    "light.radius"
    "currentTime"
    "viewportSize"
	
set - [optional] id of set in shader. Initial 0.(maybe need to parse form shader)
binding - [required] id within set(maybe need to parse form shader)
	
Example:
<var name="transform.projectionMatrix" val="transform.projectionMatrix" binding="0"/>
<var name="transform.modelMatrix" val="transform.modelMatrix" binding="0"/>
<var name="transform.viewMatrix" val="transform.viewMatrix" binding="0"/>
<var name="finSize" type="float" val="5.5" binding="1"/>
<var name="finPosition" type="vec3" val="1.0, 1.0, 1.0" binding="1"/>

...