#pragma once



#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Material.h"

/*
** VERTEX CLASS
*/
class Vertex {
public:

	Vertex() {
		m_coord = glm::vec3();
	}

	Vertex(const glm::vec3& coord) {
		m_coord = coord;
	}

	inline bool operator<(const Vertex& other) const
	{
		return m_coord.x != other.m_coord.x ? m_coord.x < other.m_coord.x
			: m_coord.y != other.m_coord.y ? m_coord.y < other.m_coord.y
			: m_coord.z < other.m_coord.z;
	}

	glm::vec3 getCoord() const { return m_coord; }
	void setCoord(const glm::vec3& coord) { m_coord = coord; }

protected:
private:
	glm::vec3 m_coord;
};

/*
**	Mesh Class
*/

class Mesh
{
public:

	enum MeshType
	{
		TRIANGLE,
		QUAD,
		CUBE,
		SPHERE1,
		SPHERE2,
		SPHERE3,
	};

	Mesh();
	void breakDown(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, int count,int limit, std::vector<glm::vec3>& result);
	Mesh(MeshType type);
	Mesh(std::vector<Vertex> & vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<GLuint>& indices);

	void CreateMesh(std::vector<Vertex>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<GLuint>& indices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

	/*
	** GET AND SET METHODS
	*/
	glm::vec3 getPos() const { return getTranslate()[3]; }
	glm::mat4 getModel() const { return getTranslate() * getRotate() * getScale(); }
	
	glm::mat4 getTranslate() const { return m_translate; }
	void setTranslate(glm::mat4 trans) { m_translate = trans; }

	glm::mat4 getRotate() const { return m_rotate; }
	//void setRotate(glm::mat4 rot) { m_rotate = rot; }

	glm::mat4 getScale() const { return m_scale; }
	void setScale(glm::mat4 scale) { m_scale = scale; }

	Shader getShader() const { return m_shader; }

	//texture
	void setTexture(const char * texLoc) { tex = Texture(texLoc); hasTex = true; }
	Texture getTexture() { return tex; }
	void loadTexture() { if (hasTex) { tex.loadTexture(); } }
	void useTexture() { tex.useTexture(); }

	//material 
	void setMaterial(Material m) { mat = m; }
	Material getMaterial() { return mat; }

	// get buffers and array references
	GLuint getVertexArrayObject() const { return VAO; }
	GLuint getVertexBuffer() const { return VBO; }
	GLuint getNormalBuffer() const { return NBO; }
	GLuint getIndexBuffer() const { return IBO; }

	// get number of vertices
	unsigned int getNumIndices() const { return numIndices; }
	
	//set initial values of all meshes
	void initTransform();

	// create vector of unique vertices (no duplicates) from vector of all mesh vertices
	//void createUniqueVertices();

	std::vector<Vertex> getVertices() { return m_vertices; }

	// set position of mesh center to specified 3D position vector
	void setPos(const glm::vec3 &position) {
		m_translate[3][0] = position[0];
		m_translate[3][1] = position[1];
		m_translate[3][2] = position[2];
	}
	// set i_th coordinate of mesh center to float p (x: i=0, y: i=1, z: i=2)
	void setPos(int i, float p) { m_translate[3][i] = p; }
	void setMatIdx(int idx) { matIdx = idx; }
	int getMatIdx() { return matIdx; }

	// set rotation matrix
	void setRotate(const glm::mat4 &mat) { m_rotate = mat; }

	// allocate shader to mesh
	void setShader(const Shader &shader) {
		m_shader = shader;
		m_shader.UseShader();
	}

	// translate mesh by a vector
	void translate(const glm::vec3 &vect);
	// rotate mesh by a vector
	void rotate(const float &angle, const glm::vec3 &vect);
	// scale mesh by a vector
	void scale(const glm::vec3 &vect);

private:
	//VAO-holds the information VBO-specific vertex IBO- idex of buffers
	GLuint VAO, VBO, NBO, IBO, TBO;
	GLsizei indexCount;

	int matIdx;
	Texture tex;
	bool hasTex;
	Material mat;

	unsigned int numIndices;
	unsigned int numVertices;

	void createNormals(std::vector<glm::vec3> & normals, std::vector<Vertex> & vertices, int numVerts);

	glm::mat4 m_translate; // translation matrix
	glm::mat4 m_rotate; // rotation matrix
	glm::mat4 m_scale; // scale matrix

	std::vector<Vertex> m_vertices; // mesh vertices (without duplication)
	Shader m_shader; // shader
};

