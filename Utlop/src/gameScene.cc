#include "gameScene.h"

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
    for each (GameObject gameObject in _gameObjects)
    {
      gameObject.draw();
    }
  }

  void GameScene::start() { }

  void GameScene::update() { }

  void GameScene::_start()
  {
    start();
    for each (GameObject gameObject in _gameObjects)
    {
      gameObject.start();
    }
  }

  void GameScene::_update()
  {
    update();
    for each (GameObject gameObject in _gameObjects)
    {
      gameObject.update();
    }
  }
}