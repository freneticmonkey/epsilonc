layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec4 colour;
layout(location=3) in vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 ex_Colour;

void main()
{
	// Set the normals as the default colour
	//ex_Colour = colour;//vec4(normal.z, 0.0f, 0.0f, 1.0f);
	ex_Colour = material.diffuse;
	gl_Position = projectionMatrix * transpose(viewMatrix) * transpose(modelMatrix) * vec4(position, 1.0);
	
}