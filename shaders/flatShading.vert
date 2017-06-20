#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexture;

flat out vec3 FragPos;
flat out vec3 Normal;
out vec2 TexCoord;

//uniform mat4 MVP;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	//gl_Position = MVP * vec4(VertexPosition, 1.0);
	gl_Position = projection * view * model * vec4(VertexPosition, 1.0);

	FragPos = vec3(model * vec4(VertexPosition, 1.0));
	
	Normal = mat3(transpose(inverse(model))) * VertexNormal;  

	TexCoord = vec2(VertexTexture.x, VertexTexture.y);
}