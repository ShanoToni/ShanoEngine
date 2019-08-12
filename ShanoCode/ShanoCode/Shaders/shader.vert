#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normals;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

out vec4 vCol;
out vec3 normal;
out vec2 TexCoord;
out vec3 FragPos;

void main()
{
	vCol = vec4(clamp(pos,0.0f, 1.0f), 1.0f);
	gl_Position =  Projection * View * Model * vec4(pos.x,pos.y,pos.z, 1.0);

	normal = mat3(transpose(inverse(Model))) * normals;
	TexCoord = texCoords;
	FragPos = (Model * vec4(pos, 1.0)).xyz;
}

