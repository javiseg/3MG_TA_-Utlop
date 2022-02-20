#include "gameObject.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "geometry.h"
#include <fstream>
#include <vector>
#include <chrono>
#include "glm/gtc/matrix_transform.hpp"
#include "core.h"

//Testing
#include <time.h>
///


namespace Utlop
{
  GameObject::GameObject()
  {
		projection_mat_index_ = -1;
  }

  GameObject::~GameObject()
  {

  }

  void GameObject::init()
  {
		mesh_ = std::make_shared<Mesh>();
		shader_ = make_shared<Shader>();
		shader_->loadShaderFiles("../UtlopTests/src/shaders/vs.glsl", "../UtlopTests/src/shaders/fs.glsl");

  }

  void GameObject::draw()
  {
    mesh_->draw(*shader_);
  }


  void GameObject::update()
  {
    
		projection_mat_index_ = shader_->uniformLocation("u_vp_matrix");
		
		glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
		glm::mat4(1.0f), transform_.getScale()), transform_.getRotationAngle(), transform_.getRotation()), transform_.getPosition());
			
		model_matrix *= Core::Instance()->getCamera()->getViewProjection();
		glUniformMatrix4fv(projection_mat_index_, 1, GL_FALSE, &model_matrix[0][0]);
  
		mesh_->update();
	}

  void GameObject::destroy()
  {

  }

  void GameObject::setTransform(Transform transform)
  {
    transform_ = transform;
  }
	void GameObject::setScale(glm::vec3 scale)
	{
		transform_.setScale(scale);
	}
	void GameObject::setRotation(glm::vec3 rotation)
	{
		transform_.setRotation(rotation);
	}
	void GameObject::setPosition(glm::vec3 position)
	{
		transform_.setPosition(position);
	}
	void GameObject::setColor(glm::vec3 color)
	{
		shader_->setParameters(U_COLOR, T_FLOAT_3, &color[0]);
	}
	void GameObject::translate(glm::vec3 position, float speed)
	{
		transform_.setPosition(transform_.getPosition() + (position * speed));
	}

	void GameObject::setGeometry(Utlop::Geo geometry)
	{
		mesh_->init(geometry);
	}

	

	void GameObject::setGeometry(char* src)
	{
		
		mesh_->init(Utlop::Geo::kConst_OBJ, src);
	}

	void GameObject::setShader(char* vertex, char* fragment)
	{
		shader_->loadShaderFiles(vertex, fragment);
	}

	GameObject& GameObject::operator=(const GameObject& other)
	{
		transform_ = other.transform_;
		mesh_ = other.mesh_;
		shader_ = other.shader_;

		return *this;
	}
	// TEXTURES STB _IMAGE
	GameObject::GameObject(const GameObject& other) {

		transform_ = other.transform_;
		mesh_ = other.mesh_;
		shader_ = other.shader_;
	}
	GameObject::GameObject(GameObject&& other) {

		transform_ = other.transform_;
		mesh_ = other.mesh_;
		shader_ = other.shader_;
	}

}