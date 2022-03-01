#include "mesh.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <fstream>
#include "gameScene.h"
#include "glm/gtc/matrix_transform.hpp"
#include <chrono>
#include "core.h"

namespace Utlop
{


  Mesh::Mesh()
  {
		vao_ = 0;
		vbo_ = 0;
		ebo_ = 0;
		tbo_ = 0;
		etbo_ = 0;
		transform_.setPosition(vec3(0.0f));
		transform_.setScale(vec3(1.0f));
		transform_.setRotation(vec3(0.0f));
		transform_.rotation_angle(0.0f);
		origin_ = vec3(0.0f);
		
  }

	Mesh::Mesh(char* src, Geo type, vector<float> vertices, vector<GLuint> verticesIndices, vector<float> texCoords, vector<GLuint> texIndices, vector<float> normals, vector<GLuint> normalsIndices)
	{
		vertices_ = vertices;
		verticesIndices_ = verticesIndices;

		texCoords_ = texCoords;
		texIndices_ = texIndices;

		normals_ = normals;
		normalsIndices_ = normalsIndices;
		objPath_ = src;

		type_ = type;

		transform_.setPosition(vec3(0.0f, 0.0f, 0.0f));
		transform_.setScale(vec3(1.0f));
		transform_.setRotation(vec3(0.0f));
		transform_.rotation_angle(0.0f);
		origin_ = vec3(0.0f);

		setupMesh();
	}

  Mesh::~Mesh()
  {

  }


	void Mesh::draw(Shader& shader)
	{

		UpdateModelMatrix();
		UpdateUniforms(shader);
		shader.use();

		
		/*int size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		*/
		if (texCoords_.size() > 0 && GameScene::getCurrentScene()->textureData_.size() > 0) {
			glBindTextureUnit(0, GameScene::getCurrentScene()->textureData_[1]->id_);
			glUniform1i(glGetUniformLocation(shader.ID(), "ourTexture"), 0);
		}
		else {
			glBindTextureUnit(0, GameScene::getCurrentScene()->textureData_[0]->id_);
			glUniform1i(glGetUniformLocation(shader.ID(), "ourTexture"), 0);
		}
		glBindVertexArray(vao_);

		glDrawElements(GL_TRIANGLES, verticesIndices_.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
		

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}


  void Mesh::start()
  {

  }

  void Mesh::update()
  {
		
  }

  void Mesh::destroy()
  {

  }

	

	void Mesh::setupMesh()
	{
		glCreateVertexArrays(1, &vao_);
		glCreateBuffers(1, &vbo_);
		glCreateBuffers(1, &ebo_);

		glNamedBufferData(vbo_,	vertices_.size() * sizeof(float),
			&vertices_[0], GL_STATIC_DRAW);
		

		glNamedBufferData(ebo_, verticesIndices_.size() * sizeof(unsigned int),
			&verticesIndices_[0],	GL_STATIC_DRAW);

		glEnableVertexArrayAttrib(vao_, 0);
		// Back here when applying normals etc:
		glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, 0);
		//

		glVertexArrayVertexBuffer(vao_, 0, vbo_, 0, 3 * sizeof(unsigned int));

		glVertexArrayElementBuffer(vao_, ebo_);


		glCreateBuffers(1, &tbo_);
		glCreateBuffers(1, &etbo_);

		if (texCoords_.size() > 0) {
			glNamedBufferData(tbo_, texCoords_.size() * sizeof(float), &texCoords_[0], GL_STATIC_DRAW);
			glNamedBufferData(etbo_, texIndices_.size() * sizeof(GLuint), &texIndices_[0], GL_STATIC_DRAW);

			glEnableVertexArrayAttrib(vao_, 3);
			glVertexArrayAttribBinding(vao_, 3, 0);
			glVertexArrayAttribFormat(vao_, 3, 2, GL_FLOAT, GL_FALSE, 0);

			glVertexArrayVertexBuffer(vao_, 3, tbo_, 0, 3 * sizeof(unsigned int));
			glVertexArrayElementBuffer(vao_, etbo_);

		}

	}

	void Mesh::UpdateModelMatrix()
	{
		this->ModelMatrix = glm::mat4(1.f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin_);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->transform_.getRotation().x), glm::vec3(1.0f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->transform_.getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->transform_.getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->transform_.getPosition() - this->origin_);
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->transform_.getScale());
	}

	void Mesh::UpdateUniforms(Shader& shader)
	{
		shader.setMat4fv(ModelMatrix, "ModelMatrix");
	}

	void Mesh::SetTransform(Transform tr)
	{
		transform_ = tr;
	}

	void Mesh::SetOrigin(vec3 origin)
	{
		origin_ = origin;
	}

	Mesh& Mesh::operator=(const Mesh& other)
	{
		vao_ = other.vao_;
		vbo_ = other.vbo_;
		ebo_ = other.ebo_;
		tbo_ = other.tbo_;
		etbo_ = other.etbo_;
		vertices_ = other.vertices_;
		verticesIndices_ = other.verticesIndices_;
		texCoords_ = other.texCoords_;
		texIndices_ = other.texIndices_;
		normals_ = other.normals_;
		normalsIndices_ = other.normalsIndices_;
		objPath_ = other.objPath_;
		type_ = other.type_;
		origin_ = other.origin_;
		transform_ = other.transform_;
		ModelMatrix = other.ModelMatrix;
		//material_ = other.material_;

		return *this;
	}


	Mesh::Mesh(std::shared_ptr<Mesh> other)
	{
		vao_ = other->vao_;
		vbo_ = other->vbo_;
		ebo_ = other->ebo_;
		tbo_ = other->tbo_;
		etbo_ = other->etbo_;
		vertices_ = other->vertices_;
		verticesIndices_ = other->verticesIndices_;
		texCoords_ = other->texCoords_;
		texIndices_ = other->texIndices_;
		normals_ = other->normals_;
		normalsIndices_ = other->normalsIndices_;
		objPath_ = other->objPath_;
		type_ = other->type_;
		origin_ = other->origin_;
		transform_ = other->transform_;
		ModelMatrix = other->ModelMatrix;
	}

	Mesh::Mesh(const Mesh& other)
	{
		vao_ = other.vao_;
		vbo_ = other.vbo_;
		ebo_ = other.ebo_;
		tbo_ = other.tbo_;
		etbo_ = other.etbo_;
		vertices_ = other.vertices_;
		verticesIndices_ = other.verticesIndices_;
		texCoords_ = other.texCoords_;
		texIndices_ = other.texIndices_;
		normals_ = other.normals_;
		normalsIndices_ = other.normalsIndices_;
		objPath_ = other.objPath_;
		type_ = other.type_;
		origin_ = other.origin_;
		transform_ = other.transform_;
		ModelMatrix = other.ModelMatrix;
		//material_ = material_ = other.material_;
	}

	Mesh::Mesh(Mesh&& other)
	{
		vao_ = other.vao_;
		vbo_ = other.vbo_;
		ebo_ = other.ebo_;
		tbo_ = other.tbo_;
		etbo_ = other.etbo_;
		vertices_ = other.vertices_;
		verticesIndices_ = other.verticesIndices_;
		texCoords_ = other.texCoords_;
		texIndices_ = other.texIndices_;
		normals_ = other.normals_;
		normalsIndices_ = other.normalsIndices_;
		objPath_ = other.objPath_;
		type_ = other.type_;
		origin_ = other.origin_;
		transform_ = other.transform_;
		ModelMatrix = other.ModelMatrix;

		//material_ = other.material_;
	}

	float normalized(float value, float max, float min) {
		return (value - min) / (max - min);
	}

	bool loadOBJ(const char* path,
		std::vector <float>& out_vertices,
		std::vector <float>& out_texCoords,
		std::vector <float>& out_normals,
		std::vector <unsigned int>& out_indices,
		std::vector <unsigned int>& out_texIndices,
		std::vector <unsigned int>& out_normalIndices, 
		std::vector <Vertex>& out_vertices_total) {

		std::vector< float > temp_vertices;


		float max = 1.0f, min = -1.0f;

		FILE* file = fopen(path, "r");
		if (file == NULL) {
			printf("Impossible to open the file !\n");
			return false;
		}
		unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
		while (1) {

			char lineHeader[128];
			// Lee la primera palabra de la línea
			int res = fscanf(file, "%s\n", lineHeader);
			if (res == EOF)
				break;


			//printf("%s\n", lineHeader);
			if (strcmp(&lineHeader[0], "v") == 0) {
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

				if (vertex.x > max || vertex.y > max || vertex.z > max) {
					max = std::max(vertex.x, vertex.y);
					max = std::max(max, vertex.z);
				}
				if (vertex.x < min || vertex.y < min || vertex.z < min) {
					min = std::min(vertex.x, vertex.y);
					min = std::min(min, vertex.z);
				}

				temp_vertices.push_back(vertex.x);
				temp_vertices.push_back(vertex.y);
				temp_vertices.push_back(vertex.z);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				out_texCoords.push_back(uv.x);
				out_texCoords.push_back(uv.y);

			}
			else if (strcmp(lineHeader, "vn") == 0) {
				glm::vec3 normal;
				fscanf(file, "%03f %03f %03f\n", &normal.x, &normal.y, &normal.z);
				out_normals.push_back(normal.x);
				out_normals.push_back(normal.y);
				out_normals.push_back(normal.z);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;

				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}

				out_indices.push_back(vertexIndex[0] - 1);
				out_indices.push_back(vertexIndex[1] - 1);
				out_indices.push_back(vertexIndex[2] - 1);
				out_texIndices.push_back(uvIndex[0] - 1);
				out_texIndices.push_back(uvIndex[1] - 1);
				out_texIndices.push_back(uvIndex[2] - 1);
				out_normalIndices.push_back(normalIndex[0] - 1);
				out_normalIndices.push_back(normalIndex[1] - 1);
				out_normalIndices.push_back(normalIndex[2] - 1);

			}
		}
		for (int i = 0; i < temp_vertices.size(); i++) {
			out_vertices.push_back(normalized(temp_vertices[i], max, min));
		}

		return true;
	}
	Utlop::Mesh* loadMeshFromOBJ(char* path) {
		std::vector <float> vertices;
		std::vector <float> texCoords;
		std::vector <float> normals;
		std::vector <unsigned int> indices;
		std::vector <unsigned int> texIndices;
		std::vector <unsigned int> normalIndices;
		std::vector<Vertex> vertices_total;

		if (loadOBJ(path, vertices, texCoords, normals, indices,
			texIndices, normalIndices, vertices_total)) {
			Utlop::Mesh m;
			m.vertices_ = vertices;
			m.verticesIndices_ = indices;
			m.texCoords_ = texCoords;
			m.texIndices_ = texIndices;
			m.normals_ = normals;
			m.normalsIndices_ = normalIndices;
			m.type_ = kConst_OBJ;
			return new Utlop::Mesh(path, kConst_OBJ, vertices, indices, texCoords, texIndices, normals, normalIndices);
		}
		else
			return nullptr;

	}
}