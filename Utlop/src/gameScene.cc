#include "gameScene.h"
#include "GLFW/glfw3.h"
#include "core.h"
#include "imgui.h"

namespace Utlop
{
  GameScene* GameScene::current_scene_ = nullptr;
  std::vector<GameScene*> GameScene::scenes_ = std::vector<GameScene*>();

  GameScene::GameScene()
  {
    scenes_.push_back(this);
  }

  GameScene::~GameScene()
  {

  }
	GameScene* GameScene::getCurrentScene() {
		return current_scene_;
	}
	unsigned int GameScene::CreateGeometry()
	{
		geometryData_.push_back(std::make_shared<Utlop::Geometry>((int)geometryData_.size()));
		return geometryData_.size()-1;
	}
	std::vector<std::shared_ptr<Utlop::Geometry>> GameScene::getGeometries()
	{
		return geometryData_;
	}
	std::shared_ptr<Utlop::Geometry> GameScene::getGeometryByID(unsigned int id)
	{
		if (id < geometryData_.size()) {
			return geometryData_[id];
		}
		else {
			return nullptr;
		}
	}
	int GameScene::getGeometryID(unsigned int id)
	{
		for each (auto& geometry in geometryData_) {
			if (geometry->getID() == id) {
				return geometry->getID();
			}
		}
		return -1;
	}
	int GameScene::getGeometryByType(Geo type)
	{
		for (int i = 0; i < geometryData_.size(); i++) {
			if (geometryData_[i]->getType() == type) {
				return geometryData_[i]->getID();
			}
		}
		return -1;
	}
	int GameScene::getGeometryByType(Geo type, char* src)
	{
		for (int i = 0; i < geometryData_.size(); i++) {
			if (geometryData_[i]->getType() == type && geometryData_[i]->IsSameOBJ(src)) {
				return geometryData_[i]->getID();
			}
		}
		return -1;
	}
	void GameScene::addGameObject(Utlop::GameObject gO)
	{
		gameObjects_.push_back(std::make_shared<GameObject>(gO));
	}
	void GameScene::ImGUI()
	{
		/*if (ImGui::Begin("Utlop Test")) {

			ImGui::SetWindowPos(ImVec2(960, 0), ImGuiCond_FirstUseEver);
			ImGui::SetWindowSize(ImVec2(400, 704));

			
			ImGui::End();


			ImGui::Render();
		}*/
	}
  void GameScene::init()
  {
    current_scene_ = this;
		polygon_ = false;
  }

  void GameScene::draw()
  {
    for each(auto& gameObject_ in gameObjects_)
    {
      gameObject_->draw();
    }
  }

  void GameScene::start() { }

  void GameScene::update() { }

  void GameScene::_start()
  {
    start();
		for each (auto & gameObject_ in GameScene::getCurrentScene()->gameObjects_)
		{
			gameObject_->setColor(glm::vec3(0.5, 1.0f, 1.0f));
			//gameObject_->setTexture("../UtlopTests/src/textures/texture.jpg");
		}
		/*ImGui::CreateContext();
		ImGui::StyleColorsDark();*/

  }
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_W) {
			Core::Instance()->getCamera()->moveForward(0.3f);
		}
		if (key == GLFW_KEY_S) {
			Core::Instance()->getCamera()->moveForward(-0.3f);
		}
		if (key == GLFW_KEY_D) {
			Core::Instance()->getCamera()->moveRight(-0.3f);
		}
		if (key == GLFW_KEY_A) {
			Core::Instance()->getCamera()->moveRight(0.3f);
		}
		if (key == GLFW_KEY_Q) {
			Core::Instance()->getCamera()->moveUp(-0.3f);
		}
		if (key == GLFW_KEY_E) {
			Core::Instance()->getCamera()->moveUp(0.3f);
		}
		if (key == GLFW_KEY_J) {
			Core::Instance()->getCamera()->RotateCamera(Core::Instance()->getDeltaTime(), 100.0f, 0);
		}
		if (key == GLFW_KEY_L) {
			Core::Instance()->getCamera()->RotateCamera(Core::Instance()->getDeltaTime(), 100.0f, 1);
		}
		if (key == GLFW_KEY_I) {
			Core::Instance()->getCamera()->RotateCamera(Core::Instance()->getDeltaTime(), 100.0f, 2);
		}
		if (key == GLFW_KEY_K) {
			Core::Instance()->getCamera()->RotateCamera(Core::Instance()->getDeltaTime(), 100.0f, 3);
		}
		if (key == GLFW_KEY_C) {
			for each (auto& gameObject_ in GameScene::getCurrentScene()->gameObjects_)
			{
				gameObject_->setGeometry(Utlop::Geo::kConst_Cube);
			}
		}
		if (key == GLFW_KEY_T) {
			for each (auto& gameObject_ in GameScene::getCurrentScene()->gameObjects_)
			{
				gameObject_->setGeometry(Utlop::Geo::kConst_Triangle);
			}
		}
		if (key == GLFW_KEY_O) {
			for each (auto& gameObject_ in GameScene::getCurrentScene()->gameObjects_)
			{
				gameObject_->setGeometry("../UtlopTests/src/obj/robot.obj");
			}
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			if (GameScene::getCurrentScene()->polygon_) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			GameScene::getCurrentScene()->polygon_ = !GameScene::getCurrentScene()->polygon_;
		}
	}
  void GameScene::_update()
  {
    update();
		//glfwPollEvents();
		glfwSetKeyCallback(Core::Instance()->getWindow()->getWindow(), key_callback);
		
		for each (auto & gameObject_ in gameObjects_)
		{
			gameObject_->update();
		}

  }
}