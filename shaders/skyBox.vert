#version 330 core
layout (location = 0) in vec3 VertexPosition;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = VertexPosition;
    vec4 pos = projection * view * vec4(VertexPosition, 1.0);
    gl_Position = pos.xyww;
}  