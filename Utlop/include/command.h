#pragma once


namespace Utlop {


	struct Command {
		virtual void executeOnGPU() = 0;
	};

	struct DrawMaterial : public Command {
		void executeOnGPU() override {

		}
	};


}