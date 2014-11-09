// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;

uniform mat4 modelMatrix;

void main()
{
	mat4 DVP = projectionMatrix * viewMatrix * transpose(modelMatrix);
	gl_Position =  DVP * vec4(position,1);
}