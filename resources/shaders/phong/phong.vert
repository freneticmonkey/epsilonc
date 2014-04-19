layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec4 colour;
layout(location=3) in vec2 texCoord;

// uniform mat4 modelViewMatrix;
// uniform mat4 projMatrix;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 ex_Colour;
out vec3 norm;
out vec3 lightDir;

void main()
{
	vec4 worldPosition = transpose(modelMatrix) * vec4(position,1.0);
	lightDir = vec3(light.position - worldPosition.xyz);

	// Set the normals as the default colour
	norm = normal;
	ex_Colour = material.diffuse;	
	// gl_Position = projMatrix * modelViewMatrix * vec4(position,1.0);

	gl_Position = projectionMatrix * transpose(viewMatrix) * transpose(modelMatrix) * vec4(position, 1.0);

	
}