#include "gameScene.h"
#include "geometric_object.h"
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
		Utlop::geometric_object cube;
		cube.init(Utlop::GEO_CUBE);
		cube.showVertices();
  }

  void GameScene::_update()
  {
    update();
		for each (GameObject _gameObject in _gameObjects)
		{
			_gameObject.update();
		}
  }
}