#include "..\include\UGameScene.h"

namespace Utlop {

	UGameScene* UGameScene::current_scene_ = nullptr;
	std::vector<UGameScene*> UGameScene::scenes_ = std::vector<UGameScene*>();

	Utlop::UGameScene::UGameScene()
	{
		render = new RenderCtx();
		current_scene_ = this;
		scenes_.push_back(this);
	}
	Utlop::UGameScene::~UGameScene()
	{
		delete render;
	}
}

