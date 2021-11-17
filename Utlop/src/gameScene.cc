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
		glm::vec3 position = { 30.0f,10.0f,0.0f };
		Transform tr;
		tr.setPosition(position);
    GameObject go;
    go.init();
		GameObject go2;
		go2.setTransform(tr);
		go2.init();
		

		_gameObjects.push_back(go2);
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