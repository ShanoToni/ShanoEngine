#version 430

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(vec3(pos.x, pos.y, pos.z), 1.0);
	TexCoord = vec2(texCoords.x, texCoords.y);
}