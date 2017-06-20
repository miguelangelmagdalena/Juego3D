#version 330

in vec2 texCoord;
out vec4 outputColor;

uniform sampler2D gSampler;
uniform vec4 vColor;

void main()
{
	vec4 vTexColor = texture2D(gSampler, texCoord);
	outputColor = vTexColor*vec4(vColor.r, vColor.r, vColor.r, vColor.r);
}