#include "gameScene.h"
#include "core.h"

namespace Utlop
{
  GameScene* GameScene::_current_scene = nullptr;
  std::vector<GameScene*> GameScene::_scenes = std::vector<GameScene*>();

  GameScene::GameScene()
  {
    _scenes.push_back(this);
  }

  GameScene::~GameScene()
  {

  }
	GameScene* GameScene::getCurrentScene() {
		return _current_scene;
	}
	std::shared_ptr<Utlop::Geometry> GameScene::CreateGeometry()
	{
		return std::make_shared<Utlop::Geometry>((int)geometryData_.size());
	}
	std::vector<std::shared_ptr<Utlop::Geometry>> GameScene::getGeometries()
	{
		return geometryData_;
	}
  void GameScene::init()
  {
    _current_scene = this;
  }

  void GameScene::draw()
  {
    for each(GameObject _gameObject in gameObjects_)
    {
      _gameObject.draw();
    }
  }

  void GameScene::start() { }

  void GameScene::update() { }

  void GameScene::_start()
  {
    start();
		for(int i = 0; i < gameObjects_.size(); i++)
		{
			gameObjects_[i].start();
			//_gameObjects[i].setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
			gameObjects_[i].setColor(glm::vec3(0.5f, 0.0f, 1.0f));
		}
		
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
			for each (GameObject _gameObject in GameScene::getCurrentScene()->gameObjects_)
			{
				_gameObject.setGeometry(Utlop::Geo::kConst_Cube);
			}
		}
		if (key == GLFW_KEY_T) {
			for each (GameObject _gameObject in GameScene::getCurrentScene()->gameObjects_)
			{
				_gameObject.setGeometry(Utlop::Geo::kConst_Triangle);
			}
		}
		if (key == GLFW_KEY_O) {
			for each (GameObject _gameObject in GameScene::getCurrentScene()->gameObjects_)
			{
				_gameObject.setGeometry("../UtlopTests/src/obj/lego.obj");
			}
		}
	}
  void GameScene::_update()
  {
    update();
		//glfwPollEvents();
		glfwSetKeyCallback(Core::Instance()->getWindow()->getWindow(), key_callback);
		
		for each (GameObject _gameObject in gameObjects_)
		{
			_gameObject.update();
		}

  }
}