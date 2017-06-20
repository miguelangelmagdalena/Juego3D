#version 400
	
out vec4 FragColor;

uniform vec3 colorFill; 

void main()
{
	FragColor = vec4(colorFill, 1.0);
}