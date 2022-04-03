#pragma once
#include "data.h"

namespace Utlop {



	struct Command {
		virtual void executeOnGPU(RenderCtx* data) = 0;
	};

	struct DrawMaterial : public Command {
		void executeOnGPU(RenderCtx* data) override {

		}
	};

	struct WindowClearCmd : public Command {
		void executeOnGPU(RenderCtx* data) override {

		}
		float r, g, b, a;
	};


}