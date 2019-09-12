#include "Model.h"


using namespace Assimp;

Model::Model()
{
}

void Model::loadModel(const std::string & filename)
{
	Importer importer;
	const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices );

	if (!scene)
	{
		std::cout << "Model failed to load: " << filename << " " << importer.GetErrorString() << std::endl;
		return;
	}

	loadNode(scene->mRootNode, scene);

	loadMaterials(scene);

}




void Model::ClearModel()
{
	for (int i = 0; i < meshlist.size(); i++)
	{
		if (meshlist[i])
		{
			delete meshlist[i];
			meshlist[i] = nullptr;
		}
	}

	for (int i = 0; i < texList.size(); i++)
	{
		if (texList[i])
		{
			delete texList[i];
			texList[i] = nullptr;
		}
	}
}


Model::~Model()
{
	ClearModel();
}

void Model::translate(const glm::vec3 & vect)
{
	for (auto mesh : meshlist)
	{
		mesh->setTranslate(glm::translate(mesh->getTranslate(), vect));
	}
}

void Model::rotate(const float & angle, const glm::vec3 & vect)
{
	for (auto mesh : meshlist)
	{
		mesh->setRotate(glm::rotate(mesh->getRotate(), angle, vect));
	}
}

void Model::scale(const glm::vec3 & vect)
{
	for (auto mesh : meshlist)
	{
		mesh->setScale(glm::scale(mesh->getScale(), vect));
	}
}

void Model::addShader(Shader shader)
{
	for (auto mesh : meshlist) {
		mesh->setShader(shader);
	}
}

void Model::loadTexture()
{
	for (auto mesh : meshlist) 
	{
		mesh->loadTexture();
	}
}

void Model::loadNode(aiNode * node, const aiScene * scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		loadNode(node->mChildren[i], scene);
	}
}

void Model::loadMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<GLuint> indices;
	std::vector<Vertex> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	
	glm::vec3 meshPos = glm::vec3(0.0f);
	for (int i =0; i< mesh->mNumVertices; i++)
	{
		meshPos += glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
	}
	meshPos = meshPos /(1.0f *  mesh->mNumVertices);

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.push_back( Vertex(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z)));

		if (mesh->mTextureCoords[0])
		{
			texCoords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
		}

		normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh * newMesh = new Mesh(vertices, normals, texCoords, indices, meshPos);
	newMesh->setMatIdx(mesh->mMaterialIndex);
	newMesh->setTexture("Textures/plane.jpg");
	newMesh->loadTexture();
	meshlist.push_back(newMesh);
	
}

void Model::loadMaterials(const aiScene * scene)
{
	texList.resize(scene->mNumMaterials);

	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial * material = scene->mMaterials[i];

		texList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("Textures/") + filename;				

				texList[i] = new Texture(texPath.c_str());
				if (!texList[i]->loadTexture())
				{
					std::cout << "Falied to load texture at : " << texPath << std::endl;
					delete texList[i];
					texList[i] = nullptr;
				}
				else
				{
					for (auto mesh : meshlist)
					{
						if (mesh->getMatIdx() == i)
						{
							mesh->setTexture(texPath.c_str());
							mesh->loadTexture();
						}
					}
				}
			}
		}
		

		
	}
}
