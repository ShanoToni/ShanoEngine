#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normals;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 lightSpaceMatrix;

out vec4 vCol;
out vec3 normal;
out vec2 TexCoord;
out vec3 FragPos;
out vec4 DirLightSpacePos;

void main()
{
	FragPos = (Model * vec4(pos, 1.0)).xyz;
	DirLightSpacePos = lightSpaceMatrix  * vec4(FragPos, 1.0);
	normal = mat3(transpose(inverse(Model))) * normals;
	TexCoord = texCoords;

	gl_Position =  Projection * View * Model * vec4(pos.x,pos.y,pos.z, 1.0);
}

