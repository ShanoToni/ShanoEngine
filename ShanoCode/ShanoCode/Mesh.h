#pragma once
#include <GL/glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();
private:
	//VAO-holds the information VBO-specific vertex IBO- idex of buffers
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

