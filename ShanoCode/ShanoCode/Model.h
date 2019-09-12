#pragma once

#include<vector>
#include <string>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

class Model
{
public:
	Model();

	void loadModel(const std::string& filename);
	std::vector<Mesh*> getMeshes() { return meshlist; };
	void ClearModel();

	~Model();

	// translate model by a vector
	void translate(const glm::vec3 &vect);
	// rotate model by a vector
	void rotate(const float &angle, const glm::vec3 &vect);
	// scale model by a vector
	void scale(const glm::vec3 &vect);
	void addShader(Shader shader);
	void loadTexture();
	
protected:

	void loadNode(aiNode *node, const aiScene *scene);
	void loadMesh(aiMesh *mesh, const aiScene *scene);
	void loadMaterials(const aiScene *scene);

	std::vector<Mesh*> meshlist;
	std::vector<Texture*> texList;
};

