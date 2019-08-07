#include "Mesh.h"



Mesh::Mesh()
{
	//create triangle vertices
	Vertex vertices[] = 
	{ 
		Vertex(glm::vec3(-1.0,-1.0,0.0)),
		Vertex(glm::vec3(0, 1.0, 0.0)),
		Vertex(glm::vec3(1.0, -1.0, 0.0)) 
	};

	// tirangle normals
	glm::vec3 normals[] = { glm::vec3(.0f, .0f, 1.0f), glm::vec3(.0f, .0f, 1.0f), glm::vec3(.0f, .0f, 1.0f) };

	// create vertex vector without duplicates (easy for a triangle)
	m_vertices = std::vector<Vertex>(std::begin(vertices), std::end(vertices));

	// number of vertices
	numIndices = 3;
	indexCount = 3;
	numVertices = 3;

	//indices

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);

	//create mesh
	CreateMesh(vertices, normals);

}

void Mesh::breakDown(
	glm::vec3 t1,
	glm::vec3 t2,
	glm::vec3 t3,
	int count,
	std::vector<glm::vec3>& resultVerts)
{
	if (count < 4)
	{
		//find mind point
		glm::vec3 mid12 = (t1 + t2) / 1.0f;
		glm::vec3 mid13 = (t1 + t3) / 1.0f;
		glm::vec3 mid23 = (t3 + t2) / 1.0f;

		breakDown(mid13, mid23, t3, count + 1, resultVerts);
		breakDown(t1, mid12, mid13, count + 1, resultVerts);
		breakDown(mid12, t2, mid23, count + 1, resultVerts);
		breakDown(mid12, mid23, mid13, count + 1, resultVerts);
		

	}
	else
	{
		resultVerts.push_back(glm::normalize(t1));
		resultVerts.push_back(glm::normalize(t2));
		resultVerts.push_back(glm::normalize(t3));

	}

}

glm::vec3 Mesh::normal(glm::vec3 a, glm::vec3 b, float length)
{
	//distance between a and b along axies
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	float dz = b.z - a.z;

	dx = dx * length / glm::distance(a, b);
	dy = dy * length / glm::distance(a, b);
	dz = dz * length / glm::distance(a, b);

	glm::vec3 c;

	c.x = a.x + dx;
	c.y = a.y + dy;
	c.z = a.z + dz;
	
	return c;
}

Mesh::Mesh(MeshType type)
{
	Vertex vertices[51280];
	glm::vec3 normals[2048];


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
		vertices[0] = Vertex(glm::vec3(-1.0f, 0.0f, -1.0f)); //1
		vertices[1] = Vertex(glm::vec3(1.0f, 0.0f, -1.0f));	 //2
		vertices[2] = Vertex(glm::vec3(-1.0f, 0.0f, 1.0f));  //3
		vertices[3] = Vertex(glm::vec3(1.0f, 0.0f, 1.0f));   //4

		// create normals
		normals[0] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[1] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[2] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[3] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[4] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[5] = glm::vec3(0.0f, 1.0f, 0.0f);

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
		vertices[0] = Vertex(glm::vec3(-1.0f, -1.0f, 1.0f)); //0
		vertices[1] = Vertex(glm::vec3(1.0f, -1.0f, 1.0f));	 //1
		vertices[2] = Vertex(glm::vec3(-1.0f, 1.0f, 1.0f));  //2
		vertices[3] = Vertex(glm::vec3(1.0f, 1.0f, 1.0f));   //3
		vertices[4] = Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)); //4
		vertices[5] = Vertex(glm::vec3(1.0f, -1.0f, -1.0f));  //5
		vertices[6] = Vertex(glm::vec3(-1.0f, 1.0f, -1.0f));  //6
		vertices[7] = Vertex(glm::vec3(1.0f, 1.0f, -1.0f));   //7

		//indices of cube
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		//	   .push_back( )
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
		//	   .push_back( )
		indices.push_back(1);
		indices.push_back(5);
		indices.push_back(3);
		//	   .push_back( )
		indices.push_back(5);
		indices.push_back( 3);
		indices.push_back( 7);
		//	   .push_back(	)
		indices.push_back( 4);
		indices.push_back( 0);
		indices.push_back( 6);
		//	   .push_back(	)
		indices.push_back( 0);
		indices.push_back( 6);
		indices.push_back( 2);
		//	   .push_back(	)
		indices.push_back( 4);
		indices.push_back( 5);
		indices.push_back( 0);
		//	   .push_back(	)
		indices.push_back( 5);
		indices.push_back( 0);
		indices.push_back( 1);
		//	   .push_back(	)
		indices.push_back( 2);
		indices.push_back( 3);
		indices.push_back( 6);
		//	   .push_back(	)
		indices.push_back( 3);
		indices.push_back( 6);
		indices.push_back( 7);
		//	   .push_back(	)
		indices.push_back( 5);
		indices.push_back( 4);
		indices.push_back( 7);
		//	   .push_back(	)
		indices.push_back( 4);
		indices.push_back( 7);
		indices.push_back( 6);


		//normals
		normals[0] = glm::vec3(0.0f, 0.0f, -1.0f);
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
		normals[35] = glm::vec3(1.0f, 0.0f, 0.0f);

		// number of vertices
		numVertices = 8;
		numIndices = 36;

		break;
	case SPHERE:
		glm::vec3 t1 = glm::vec3(-1.0f, 0.0f, 1.0f);
		glm::vec3 t2 = glm::vec3(1.0f, 0.0f, 1.0f);
		glm::vec3 t3 = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 t4 = glm::vec3(-1.0f, 0.0f, -1.0f);
		glm::vec3 t5 = glm::vec3(1.0f, 0.0f, -1.0f);
		glm::vec3 t6 = glm::vec3(0.0f, -1.0f, 0.0f);

		std::vector<glm::vec3> result;
		//TOP
		//front
		breakDown(t1, t2, t3, 0, result);
		//back
		breakDown(t4, t5, t3, 0, result);
		//left
		breakDown(t1, t4, t3, 0, result);
		//right
		breakDown(t2, t5, t3, 0, result);
		//BOTTOM
		//front
		breakDown(t1, t2, t6, 0, result);
		//back
		breakDown(t4, t5, t6, 0, result);
		//left
		breakDown(t1, t4, t6, 0, result);
		//right
		breakDown(t2, t5, t6, 0, result);

		for (int i = 0; i < result.size(); i++) 
		{
			vertices[i] = result.at(i);
			indices.push_back(i);
			numVertices = i+1;
			numIndices = i+1;
		}

		break;
	}

	indexCount = numIndices;

	// generate unique vertex vector (no duplicates)
	m_vertices = std::vector<Vertex>(std::begin(vertices), std::end(vertices));
	//createUniqueVertices();

	//create mesh
	CreateMesh(vertices, normals);

	// create model matrix (identity)
	initTransform();
}


void Mesh::CreateMesh(Vertex * vertices, glm::vec3* normals)
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, &vertices[0], GL_STATIC_DRAW);

	// vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

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
