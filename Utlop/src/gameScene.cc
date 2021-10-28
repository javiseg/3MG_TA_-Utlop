#include "gameScene.h"

namespace Utlop
{
  GameScene::GameScene()
  {

  }

  GameScene::~GameScene()
  {

  }

  void GameScene::init()
  {
    GameObject go;
    go.init();
    _gameObjects.push_back(go);
  }

  void GameScene::draw()
  {
    for each (GameObject gameObject in _gameObjects)
    {
      gameObject.draw();
    }
  }
  void GameScene::update()
  {
    for each (GameObject gameObject in _gameObjects)
    {
      gameObject.update();
    }
  }
}