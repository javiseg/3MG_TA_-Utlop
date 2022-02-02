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
    //GameObject go;
    //go.init();
    //_gameObjects.push_back(go);
    _current_scene = this;
    //_start();
  }

  void GameScene::draw()
  {
    for(int i = 0; i < _gameObjects.size(); i++)
    {
      _gameObjects[i].draw();
    }
  }

  void GameScene::start() { }

  void GameScene::update() { }

  void GameScene::_start()
  {
    start();
		for (int i = 0; i < _gameObjects.size(); i++)
		{
			_gameObjects[i].start();
		}
		Utlop::geometric_object cube;
		cube.init(Utlop::GEO_CUBE);
		cube.showVertices();
  }

  void GameScene::_update()
  {
    update();
		for (int i = 0; i < _gameObjects.size(); i++)
		{
			_gameObjects[i].update();
		}
  }
}