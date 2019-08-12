#include "Mesh.h"

Mesh::Mesh()
{
	std::vector<GLuint> indices;
	//create triangle vertices
	std::vector<Vertex> vertices;
	
	vertices.push_back(Vertex(glm::vec3(-1.0, -1.0, 0.0)));
	vertices.push_back(Vertex(glm::vec3(0, 1.0, 0.0)));
	vertices.push_back(Vertex(glm::vec3(1.0, -1.0, 0.0)));
	

	// tirangle normals
	std::vector<glm::vec3> normals;
		normals.push_back(glm::vec3(.0f, .0f, 1.0f));
		normals.push_back(glm::vec3(.0f, .0f, 1.0f));
		normals.push_back(glm::vec3(.0f, .0f, 1.0f));

	std::vector<glm::vec2> texCoords;
	
	 texCoords.push_back(glm::vec2(1.0f, 0.0f));
	 texCoords.push_back(glm::vec2(0.5f, 1.0f));
	 texCoords.push_back(glm::vec2(0.0f, 0.0f));

	// create vertex vector without duplicates (easy for a triangle)
	m_vertices = std::vector<Vertex>(std::begin(vertices), std::end(vertices));

	// number of vertices
	numIndices = 3;
	indexCount = 3;
	numVertices = 3;

	//indices
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(2);

	tex = Texture();
	hasTex = false;
	//create mesh
	CreateMesh(vertices, normals , texCoords, indices);

}

void Mesh::breakDown(
	glm::vec3 t1,
	glm::vec3 t2,
	glm::vec3 t3,
	int count,
	int limit,
	std::vector<glm::vec3>& resultVerts)
{
	if (count < limit)
	{
		//find mind point
		glm::vec3 mid12 = (t1 + t2) / 1.0f;
		glm::vec3 mid13 = (t1 + t3) / 1.0f;
		glm::vec3 mid23 = (t3 + t2) / 1.0f;

		breakDown(mid13, mid23, t3, count + 1,limit, resultVerts);
		breakDown(t1, mid12, mid13, count + 1,limit, resultVerts);
		breakDown(mid12, t2, mid23, count + 1,limit, resultVerts);
		breakDown(mid12, mid23, mid13, count + 1, limit, resultVerts);
		

	}
	else
	{
		resultVerts.push_back(glm::normalize(t1));
		resultVerts.push_back(glm::normalize(t2));
		resultVerts.push_back(glm::normalize(t3));
	}

}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2> &texCoords, std::vector<GLuint>& indices)
{
	numIndices = indices.size();
	numVertices = vertices.size();
	tex = Texture();
	hasTex = false;
	CreateMesh(vertices, normals, texCoords, indices);
	initTransform();
}

Mesh::Mesh(MeshType type)
{
	tex = Texture();
	hasTex = false;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector < glm::vec3 >normals;
	std::vector < glm::vec2> texCoords;
	int limit = 1;
	int i = 0;
	glm::vec3 t1 = glm::vec3(-1.0f, 0.0f, 1.0f);
	glm::vec3 t2 = glm::vec3(1.0f, 0.0f, 1.0f);
	glm::vec3 t3 = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 t4 = glm::vec3(-1.0f, 0.0f, -1.0f);
	glm::vec3 t5 = glm::vec3(1.0f, 0.0f, -1.0f);
	glm::vec3 t6 = glm::vec3(0.0f, -1.0f, 0.0f);

	std::vector<glm::vec3> result;

	switch (type)
	{
	case TRIANGLE:
		// Create triangle
		vertices[0] = Vertex(glm::vec3(-1.0, -1.0, 0.0));
		vertices[1] = Vertex(glm::vec3(0, 1.0, 0.0));
		vertices[2] = Vertex(glm::vec3(1.0, -1.0, 0.0));
		normals[0] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[1] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[2] = glm::vec3(0.0f, 1.0f, 0.0f);

		// number of vertices
		numIndices = 3;

		break;

	case QUAD:
		// create quad vertices
		vertices.push_back(Vertex(glm::vec3(-1.0f, 0.0f, -1.0f))); //1
		vertices.push_back(Vertex(glm::vec3(1.0f, 0.0f, -1.0f)));	 //2
		vertices.push_back(Vertex(glm::vec3(-1.0f, 0.0f, 1.0f)));  //3
		vertices.push_back(Vertex(glm::vec3(1.0f, 0.0f, 1.0f)));   //4

		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));

		// create normals
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

		// Indices
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);

		numIndices = 6;
		numVertices = 4;

		break;

	case CUBE:
		// create cube
		
		//back face
		vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, -1.0f)));

		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));

		//front
		vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, 1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, 1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, 1.0f)));
		vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, 1.0f)));
		vertices.push_back( Vertex(glm::vec3(1.0f, 1.0f, 1.0f)));
		vertices.push_back( Vertex(glm::vec3(-1.0f, 1.0f, 1.0f)));

		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));

		//bottom
		vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, 1.0f)));
		vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, 1.0f)));
		vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, 1.0f)));

		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));

		//top
		vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, 1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, 1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, 1.0f)));

		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));

		////left
		vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, 1.0f)));
		vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, 1.0f)));
		vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, 1.0f)));
		
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));

		//right
		vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, 1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, -1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, 1.0f)));
		vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, 1.0f)));

		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(0.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(0.0f, 0.0f));
		texCoords.push_back(glm::vec2(1.0f, 1.0f));
		texCoords.push_back(glm::vec2(1.0f, 0.0f));

		
		//normals
		/*normals[0] = glm::vec3(0.0f, 0.0f, -1.0f);
		normals[1] = glm::vec3(0.0f, 0.0f, -1.0f);
		normals[2] = glm::vec3(0.0f, 0.0f, -1.0f);
		normals[3] = glm::vec3(0.0f, 0.0f, -1.0f);
		normals[4] = glm::vec3(0.0f, 0.0f, -1.0f);
		normals[5] = glm::vec3(0.0f, 0.0f, -1.0f);
		normals[6] = glm::vec3(0.0f, 0.0f, 1.0f);
		normals[7] = glm::vec3(0.0f, 0.0f, 1.0f);
		normals[8] = glm::vec3(0.0f, 0.0f, 1.0f);
		normals[9] = glm::vec3(0.0f, 0.0f, 1.0f);
		normals[10] = glm::vec3(0.0f, 0.0f, 1.0f);
		normals[11] = glm::vec3(0.0f, 0.0f, 1.0f);
		normals[12] = glm::vec3(0.0f, -1.0f, 0.0f);
		normals[13] = glm::vec3(0.0f, -1.0f, 0.0f);
		normals[14] = glm::vec3(0.0f, -1.0f, 0.0f);
		normals[15] = glm::vec3(0.0f, -1.0f, 0.0f);
		normals[16] = glm::vec3(0.0f, -1.0f, 0.0f);
		normals[17] = glm::vec3(0.0f, -1.0f, 0.0f);
		normals[18] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[19] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[20] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[21] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[22] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[23] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[24] = glm::vec3(-1.0f, 0.0f, 0.0f);
		normals[25] = glm::vec3(-1.0f, 0.0f, 0.0f);
		normals[26] = glm::vec3(-1.0f, 0.0f, 0.0f);
		normals[27] = glm::vec3(-1.0f, 0.0f, 0.0f);
		normals[28] = glm::vec3(-1.0f, 0.0f, 0.0f);
		normals[29] = glm::vec3(-1.0f, 0.0f, 0.0f);
		normals[30] = glm::vec3(1.0f, 0.0f, 0.0f);
		normals[31] = glm::vec3(1.0f, 0.0f, 0.0f);
		normals[32] = glm::vec3(1.0f, 0.0f, 0.0f);
		normals[33] = glm::vec3(1.0f, 0.0f, 0.0f);
		normals[34] = glm::vec3(1.0f, 0.0f, 0.0f);
		normals[35] = glm::vec3(1.0f, 0.0f, 0.0f);*/

		// number of vertices
		numVertices = vertices.size();
		numIndices = vertices.size();
		createNormals(normals, vertices, numVertices);
		for (int i = 0; i< vertices.size(); i++)
		{
			indices.push_back(i);
		}

		break;
	case SPHERE1:
		
		limit = 1;
		//TOP
			//front
		breakDown(t1, t2, t3, 0, limit, result);
		//back
		breakDown(t4, t3, t5, 0, limit, result);
		//left
		breakDown(t1, t3, t4, 0, limit, result);
		//right
		breakDown(t2, t5, t3, 0, limit, result);
		//BOTTOM
		//front
		breakDown(t1, t6, t2, 0, limit, result);
		//back
		breakDown(t4, t5, t6, 0, limit, result);
		//left
		breakDown(t1, t4, t6, 0, limit, result);
		//right
		breakDown(t2, t6, t5, 0, limit, result);

		for (int i = 0; i < result.size(); i++) 
		{
			vertices.push_back(result.at(i));
			indices.push_back(i);
			numVertices = i+1;
			numIndices = i+1;
		}
		createNormals(normals, vertices, numVertices);
		for (int i = 0; i < result.size(); i++)
		{
			texCoords.push_back(glm::vec2(asin(vertices[i].getCoord().x) / 3.14159265f + 0.5f, asin(vertices[i].getCoord().y) / 3.14159265f + 0.5f));
		}
		break;

		case SPHERE2:
		 
		limit = 2;

		//TOP
			//front
		breakDown(t1, t2, t3, 0, limit, result);
		//back
		breakDown(t4, t3, t5, 0, limit, result);
		//left
		breakDown(t1, t3, t4, 0, limit, result);
		//right
		breakDown(t2, t5, t3, 0, limit, result);
		//BOTTOM
		//front
		breakDown(t1, t6, t2, 0, limit, result);
		//back
		breakDown(t4, t5, t6, 0, limit, result);
		//left
		breakDown(t1, t4, t6, 0, limit, result);
		//right
		breakDown(t2, t6, t5, 0, limit, result);

		for (int i = 0; i < result.size(); i++)
		{
			vertices.push_back(result.at(i));
			indices.push_back(i);
			numVertices = i + 1;
			numIndices = i + 1;
		}
		createNormals(normals, vertices, numVertices);
		for (int i = 0; i < result.size(); i++)
		{
			texCoords.push_back(glm::vec2(asin(vertices[i].getCoord().x) / 3.14159265f + 0.5f, asin(vertices[i].getCoord().y) / 3.14159265f + 0.5f));
		}
		break;

		case SPHERE3:
			limit = 4;
		
			//TOP
			//front
			breakDown(t1, t2, t3, 0, limit, result);
			//back
			breakDown(t4, t3, t5, 0, limit, result);
			//left
			breakDown(t1, t3, t4, 0, limit, result);
			//right
			breakDown(t2, t5, t3, 0, limit, result);
			//BOTTOM
			//front
			breakDown(t1, t6, t2, 0, limit, result);
			//back
			breakDown(t4, t5, t6, 0, limit, result);
			//left
			breakDown(t1, t4, t6, 0, limit, result);
			//right
			breakDown(t2, t6, t5, 0, limit, result);

			for (int i = 0; i < result.size(); i++)
			{
				vertices.push_back(result.at(i));
				indices.push_back(i);
				numVertices = i + 1;
				numIndices = i + 1;
			}
			createNormals(normals, vertices, numVertices);
			for (int i = 0; i < result.size(); i++)
			{
				texCoords.push_back(glm::vec2(asin(vertices[i].getCoord().x) / 3.14159265f + 0.5f, asin(vertices[i].getCoord().y) / 3.14159265f + 0.5f));
			}
			break;
		
		
	}

	indexCount = numIndices;

	// generate unique vertex vector (no duplicates)
	m_vertices = std::vector<Vertex>(std::begin(vertices), std::end(vertices));
	//createUniqueVertices();

	//create mesh
	
	CreateMesh(vertices, normals, texCoords, indices);

	// create model matrix (identity)
	initTransform();
}







void Mesh::CreateMesh(std::vector<Vertex>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<GLuint>& indices )
{
	//Create VAO and send it to GPU
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//create IBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])* numIndices, &indices[0], GL_STATIC_DRAW);

	//Create VBO 
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// Create NBO
	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);

	//Create TBO 
	if (texCoords.size()>0) 
	{
		glGenBuffers(1, &TBO);
		glBindBuffer(GL_ARRAY_BUFFER, TBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);

		// TexCoords
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, TBO);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}


	// vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	

	glBindVertexArray(0);
}

void Mesh::RenderMesh()
{

}



void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0)
	{
		glDeleteBuffers(1, &VAO);
		VAO = 0;
	}
	if (TBO != 0)
	{
		glDeleteBuffers(1, &TBO);
		TBO = 0;
	}
	if (NBO != 0)
	{
		glDeleteBuffers(1, &NBO);
		NBO = 0;
	}
	
	indexCount = 0;
}



Mesh::~Mesh()
{
	ClearMesh();
}

void Mesh::initTransform()
{
	m_translate = glm::mat4(1.0f);
	m_rotate = glm::mat4(1.0f);
	m_scale = glm::mat4(1.0f);
}

void Mesh::translate(const glm::vec3 & vect)
{
	m_translate = glm::translate(m_translate, vect);
}

void Mesh::rotate(const float & angle, const glm::vec3 & vect)
{
	m_rotate = glm::rotate(m_rotate, angle, vect);
}

void Mesh::scale(const glm::vec3 & vect)
{
	m_scale = glm::scale(m_scale, vect);
}

void Mesh::createNormals(std::vector<glm::vec3> & normals, std::vector<Vertex> & vertices, int numVerts)
{
	for (int i = 0; i < numVerts; i++)
	{
		glm::vec3 v1 = vertices[i + 1].getCoord() - vertices[i].getCoord();
		glm::vec3 v2 = vertices[i + 2].getCoord() - vertices[i].getCoord();

		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		
		i = i + 2;
	}
}
