#version 400

layout (location = 0) in vec3 VertexPosition;

uniform mat4 MVP;
//uniform mat4 projection;
//uniform mat4 view;
//uniform mat4 model;

void main()
{
	gl_Position = MVP * vec4(VertexPosition, 1.0);
	//gl_Position = projection * view * model * vec4(VertexPosition, 1.0);
}