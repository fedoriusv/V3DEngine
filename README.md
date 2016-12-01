#V3DEngine

TODO:
...

##Render Technique
RenderTechnique structure represented in xml format:

technique tag:
- name - name of render technique
- version - version of file

Example:
&lt;technique name="techniqueName" version="100"&gt;...&lt;/technique&gt;
<br>
Contains render passes, render targets, and textures tags.

##Render Pass
Single pass of render. Included in RenderTechnique tag

pass tag:
 - name - [required] name of render pass

Example:
&lt;pass name="pass0"&gt;...&lt;/pass&gt;
<br>
Contains uniforms, attributes, samplers, render states, render targets and shaders tags.

###Uniforms
List of uniforms which will be used in shader program. Included in Pass tag

var tag:
- name - [required] name of uniform. Must be same just in shader.
- type - [required/optional] type of data of uniform. Must be same just in shader. If uniform has fixed value, then this value ignored.
Used next fixed types: "int", "uint", "int64_t", "uint64_t", "float", "double", "ivec2", "uvec2", "i64vec2", "u64vec2", "vec2", "dvec2",
 "ivec3", "uvec3", "i64vec3", "u64vec3", "vec3", "dvec3", "ivec4", "uvec4", "i64vec4", "u64vec4", "vec4", "dvec4", "mat3", "dmat3", "mat4", "dmat4"
	
- val - [required/optional] initial value of uniform. If used user data, then need to specify type of data value.
Used next fixed values:"transform.projectionMatrix", "transform.modelMatrix", "transform.viewMatrix", "transform.normalMatrix",   "transform.viewPosition", "transform.viewUpVector",
 "material.ambient", "material.diffuse", "material.specular", "material.emission",  "material.shininess", "material.transparency", "lights.count", "light.position", "light.ambient",
 "light.diffuse", "light.specular",  "light.direction", "light.attenuation", "light.radius", "currentTime", "viewportSize"

Example:<br>
&lt;uniforms&gt;<br>
    &lt;var name="projectionMatrix" val="transform.projectionMatrix"/&gt;<br>
    &lt;var name="modelMatrix" val="transform.modelMatrix"/&gt;<br>
    &lt;var name="viewMatrix" val="transform.viewMatrix"/&gt;<br>
    &lt;var name="finSize" type="float" val="5.5"/&gt;<br>
    &lt;var name="finPosition" type="vec3" val="1.0, 1.0, 1.0"/&gt;<br>
&lt;/uniforms><br>

###Attributes
List of attributes(channels) which will be used in shader program. Included in Pass tag

var tag:
- name - [required] name of attribute. Must be same just in shader.
- type - [required/optional] type of data of attribute. Must be same just in shader. If attribute has fixed value, then this value ignored.
Used next fixed types: "int", "uint", "int64_t", "uint64_t", "float", "double", "ivec2", "uvec2", "i64vec2", "u64vec2", "vec2", "dvec2",
 "ivec3", "uvec3", "i64vec3", "u64vec3", "vec3", "dvec3", "ivec4", "uvec4", "i64vec4", "u64vec4", "vec4", "dvec4", "mat3", "dmat3", "mat4", "dmat4"

- val - [required/optional] value of attribute. If used user data, then need to specify type of data value. User value initialized through source code. 
Used next fixed values: "mesh.position", "mesh.color", "mesh.normal", "mesh.binormal", "mesh.tangent", "mesh.texture0", "mesh.texture1", "mesh.texture2", "mesh.texture3",
 "partical.position", "partical.color", "partical.velocity", "particals.lifetime", "partical.size", "partical.type", "attachment.color"

- div - [optional] "divisor" how often data of attributes updated every instance. (1 - every instance, n - every n instance). Default value is 1.

Example:<br>
&lt;attributes&gt;<br>
    &lt;var name="inPosition" val="mesh.position"/&gt;<br>
    &lt;var name="inNormal" val="mesh.normal"/&gt;<br>
    &lt;var name="inUV" val="mesh.texture0"/&gt;<br>
    &lt;var name="inOffset" type="vec3" div="1"/&gt;<br>
&lt;/attributes><br>

###Samplers
TODO:
...

###Shaders
Included shaders modules which will be used in shader program. Included in Pass tag

Contains shaders and defines tags.

defines tag [optional]:
- name - [required] name of define
- val - [optional] value of define

Example:<br>
&lt;defines&gt;<br>
   &lt;var name="MAGIC_VALUE" val="5"/&gt;<br>
   &lt;var name="ENABLE_SHADOW"/&gt;<br>
&lt;/defines><br>

var tag [required]:
- file - [optional] shader will be loaded from file
- name - [required] alias of shader
- type - [required] type of shader.
Used next fixed types: "vertex", "fragment", "geometry", "control", "evaluation", "compute"

...
