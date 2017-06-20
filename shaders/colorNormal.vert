#version 400

layout (location = 0) in vec3 VertexPosition;

out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(VertexPosition, 1.0);

	FragPos = vec3(model * vec4(VertexPosition, 1.0));
}