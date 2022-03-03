#pragma once
#include <vector>

namespace Utlop {

	class UGameScene
	{
		void init();
		static UGameScene* getScene();

		static UGameScene* current_scene_;
		static std::vector<UGameScene*> scenes_;
	};
}

