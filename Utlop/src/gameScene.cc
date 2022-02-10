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

  void GameScene::init()
  {
    _current_scene = this;
  }

  void GameScene::draw()
  {
    for each(GameObject _gameObject in _gameObjects)
    {
      _gameObject.draw();
    }
  }

  void GameScene::start() { }

  void GameScene::update() { }

  void GameScene::_start()
  {
    start();
		for(int i = 0; i < _gameObjects.size(); i++)
		{
			_gameObjects[i].start();
			_gameObjects[i].setPosition(glm::vec3(-0.5f, 0.0f, -0.1f));
			_gameObjects[i].setColor(glm::vec3(0.5f, 0.0f, 1.0f));
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
			
	}
  void GameScene::_update()
  {
    update();
		//glfwPollEvents();
		glfwSetKeyCallback(Core::Instance()->getWindow()->getWindow(), key_callback);
		
		for each (GameObject _gameObject in _gameObjects)
		{
			_gameObject.update();
		}
  }
}