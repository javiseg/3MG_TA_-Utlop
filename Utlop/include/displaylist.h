#pragma once
#include "command.h"
#include <list>
#include <memory>
namespace Utlop {

	using namespace std;


	struct Command {
		virtual void executeOnGPU() = 0;
	};
	struct WindowClearCmd : public Command {
		void executeOnGPU() override {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(r, g, b, a);
		}
		float r, g, b, a;
	};
	struct AddMaterialCmd : public Command {
		void executeOnGPU() override {
		}
		GLuint id;
	};
	struct DrawMaterialCmd : public Command {
		void executeOnGPU() override {
			glUseProgram(shaderId);

			glBindTextureUnit(0, materialID);

			glBindVertexArray(vao);

			glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

			glUseProgram(0);
		}
		GLuint shaderId;
		GLuint materialID;
		GLuint vao;
		size_t size;
	};
	struct SetPolygonCmd : public Command {
		void executeOnGPU() override {
			if (value)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		uint value;
	};

	void callback_WindowClearCmd(WindowClearCmd cmd);

	class DisplayList {
		public:
			list<shared_ptr<Command>> cmdList;
			void executeOnGPU(list<shared_ptr<Command>> cmdListToExecute);
			void submit();

	};


	DisplayList& addWindowClearCmd(DisplayList* dl, float r, float g, float b, float a);
	DisplayList& addInitMaterialCmd(DisplayList* dl, float r, float g, float b, float a);
	DisplayList& addDrawCmd(DisplayList* dl, GLuint shaderId, GLuint materialID, GLuint vao, size_t size);
	DisplayList& addSetPolygonCmd(DisplayList* dl, uint on);

}