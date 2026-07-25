#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class mesh
{
public: 
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;


	VAO VAO;

	mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	void Draw(Shader& shader, Camera& camera);
};
#endif