// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;
 
void main()
{
	gl_Position =  depthModelViewProjection * vec4(position,1);
}