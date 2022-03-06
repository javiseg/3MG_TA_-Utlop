#pragma once
#include "data.h"

namespace Utlop {

	class UGameScene
	{
		public:
			UGameScene();
			~UGameScene();
			void init();
			static UGameScene* getScene();

			static UGameScene* current_scene_;
			static vector<UGameScene*> scenes_;
			
			RenderCtx* render;
		
	};
}

