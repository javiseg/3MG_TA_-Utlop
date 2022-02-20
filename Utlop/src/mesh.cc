#include "mesh.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <fstream>
#include "gameScene.h"
#include <chrono>

namespace Utlop
{


  Mesh::Mesh()
  {
		vao_ = 0;
		vbo_ = 0;
		ebo_ = 0;
		id_geometry_ = -1;
		id_texture_ = -1;
		
  }

  Mesh::~Mesh()
  {

  }

  void Mesh::init(Geo type)
  {
		type_ = type;
		if (GameScene::getCurrentScene()->getGeometryByType(type_) != -1) {
			id_geometry_ = GameScene::getCurrentScene()->getGeometryByType(type_);
		}
		else {
			id_geometry_ = GameScene::getCurrentScene()->CreateGeometry();
			
			GameScene::getCurrentScene()->getGeometryByID(id_geometry_)->newGeometryData(type_);
		}
		setupMesh();

	}

	void Mesh::init(Geo type, char* src)
	{
		type_ = type;
		if (GameScene::getCurrentScene()->getGeometryByType(type_) != -1) {
			id_geometry_ = GameScene::getCurrentScene()->getGeometryByType(type_);
		}
		else {
			id_geometry_ = GameScene::getCurrentScene()->CreateGeometry();

			GameScene::getCurrentScene()->getGeometryByID(id_geometry_)->newGeometryData(src);
		}
		setupMesh();
	}

	void Mesh::draw(Shader& shader)
	{
		shader.use();
		glBindVertexArray(vao_);
		int size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		

		glDrawElements(GL_TRIANGLES, size / sizeof(int), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		glGenVertexArrays(1, &vao_);
		glGenBuffers(1, &vbo_);
		glGenBuffers(1, &ebo_);

		glBindVertexArray(vao_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, 
			GameScene::getCurrentScene()->getGeometryByID(id_geometry_)->getVertices().size() * sizeof(float),
			&GameScene::getCurrentScene()->getGeometryByID(id_geometry_)->getVertices()[0],
			GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			GameScene::getCurrentScene()->getGeometryByID(id_geometry_)->getIndices().size() * sizeof(unsigned int),
			&GameScene::getCurrentScene()->getGeometryByID(id_geometry_)->getIndices()[0],
			GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		// Back here when applying normals etc:
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//
		glBindVertexArray(0);

	}


	Mesh& Mesh::operator=(const Mesh& other)
	{
		vao_ = other.vao_;
		vbo_ = other.vbo_;

		//material_ = other.material_;

		return *this;
	}

	Mesh::Mesh(std::shared_ptr<Mesh> other)
	{
		
	}

	Mesh::Mesh(const Mesh& other)
	{
		vao_ = other.vao_;
		vbo_ = other.vbo_;

		//material_ = material_ = other.material_;
	}

	Mesh::Mesh(Mesh&& other)
	{
		vao_ = other.vao_;
		vbo_ = other.vbo_;

		//material_ = other.material_;
	}
}