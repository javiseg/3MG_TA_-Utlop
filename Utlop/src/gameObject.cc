#include "gameObject.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
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
		shader_ = make_shared<Shader>();
		shader_->loadShaderFiles("../UtlopTests/src/shaders/vs.glsl", "../UtlopTests/src/shaders/fs.glsl");
		projection_mat_index_ = shader_->uniformLocation("u_vp_matrix");
		
		shader_->setMat4fv(Core::Instance()->getCamera()->getView(), "ViewMatrix");
		shader_->setMat4fv(Core::Instance()->getCamera()->getProjection(), "ProjectionMatrix");
	}

  void GameObject::draw()
  {
		for (int i = 0; i < meshIndices_.size(); i++) {
			GameScene::getCurrentScene()->getMeshes()[meshIndices_[i]]->SetTransform(transformVector_[i]);
			GameScene::getCurrentScene()->getMeshes()[meshIndices_[i]]->draw(*shader_);
		}
  }


  void GameObject::update()
  {
		/*shader_->use();

		glm::mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
			glm::mat4(1.0f), transform_.getScale()), transform_.getRotationAngle(), transform_.getRotation()), transform_.getPosition());

		model_matrix *= Core::Instance()->getCamera()->getViewProjection();
		glUniformMatrix4fv(projection_mat_index_, 1, GL_FALSE, &model_matrix[0][0]);*/
		shader_->setMat4fv(Core::Instance()->getCamera()->getView(), "ViewMatrix");
		
	}

  void GameObject::destroy()
  {

  }

  void GameObject::setTransform(Transform transform)
  {
    transform_ = transform;
  }
  void GameObject::setMeshPosition(int meshIndex, vec3 pos)
  {
		transformVector_[meshIndex].setPosition(pos);
  }
	vec3 GameObject::getMeshPosition(int meshIndex)
	{
		return transformVector_[meshIndex].getPosition();
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


	void GameObject::addMesh(int indexmesh)
	{
		
		transformVector_.push_back(transform_);
		meshIndices_.push_back(indexmesh);
	}

	vector<GLuint> GameObject::getMeshIndices()
	{
		return meshIndices_;
	}

	void GameObject::setShader(char* vertex, char* fragment)
	{
		shader_->loadShaderFiles(vertex, fragment);
	}

	void GameObject::setTexture(char* path)
	{
		shader_->loadFragmentShader("../UtlopTests/src/shaders/fs_texture.glsl");
		Texture text(path);
		textures_.push_back(text);
		textures_.back().Bind();
		shader_->setInt("u_Texture", 3);
		
		TextureCoords text_coords;
		text_coords.setCoordsFromFile("../UtlopTests/src/obj/doc.obj");
		textureCoords_.push_back(text_coords);
	
		
	}

	GameObject& GameObject::operator=(const GameObject& other)
	{
		transform_ = other.transform_;
		shader_ = other.shader_;
		projection_mat_index_ = other.projection_mat_index_;
		textures_ = other.textures_;
		textureCoords_ = other.textureCoords_;
		meshIndices_ = other.meshIndices_;
		transformVector_ = other.transformVector_;

		return *this;
	}
	// TEXTURES STB _IMAGE
	GameObject::GameObject(const GameObject& other) {

		transform_ = other.transform_;
		shader_ = other.shader_;
		projection_mat_index_ = other.projection_mat_index_;
		textures_ = other.textures_;
		textureCoords_ = other.textureCoords_;
		meshIndices_ = other.meshIndices_;
		transformVector_ = other.transformVector_;
	}
	GameObject::GameObject(GameObject&& other) {

		transform_ = other.transform_;
		shader_ = other.shader_;
		projection_mat_index_ = other.projection_mat_index_;
		textures_ = other.textures_;
		textureCoords_ = other.textureCoords_;
		meshIndices_ = other.meshIndices_;
		transformVector_ = other.transformVector_;
	}

}