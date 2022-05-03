#pragma once
#include <list>
#include <memory>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/mat4x4.hpp"

namespace Utlop {

	using namespace std;
	using namespace glm;

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

			glBindVertexArray(vao);

			glBindTextureUnit(0, materialID);

			glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

			glUseProgram(0);
		}
		GLuint shaderId;
		GLuint materialID;
		GLuint vao;
		GLsizei size;
		glm::mat4 view;
		glm::mat4 model;
	};
	struct SetPolygonCmd : public Command {
		void executeOnGPU() override {
			if (value)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		uint8_t value;
	};
	struct SetModelViewProjectionCmd : public Command {
		void executeOnGPU() override {
			glUseProgram(shaderID);
			int mem = glGetUniformLocation(shaderID, "ModelMatrix");
			glUniformMatrix4fv(glGetUniformLocation(shaderID, "ModelMatrix"), 1, GL_FALSE, &m[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shaderID, "ViewMatrix"), 1, GL_FALSE, &v[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shaderID, "ProjectionMatrix"), 1, GL_FALSE, &p[0][0]);
			glUseProgram(0);
		}
		glm::mat4 m;
		glm::mat4 v;
		glm::mat4 p;
		GLuint shaderID;
	};
	//void callback_WindowClearCmd(WindowClearCmd cmd);
	struct SetLightDataCmd : public Command {
		void executeOnGPU() override {
			glUseProgram(shaderID);
			int p_dir = glGetUniformLocation(shaderID, "dirLight.direction");
			int p_color = glGetUniformLocation(shaderID, "dirLight.color");
			int p_intensity = glGetUniformLocation(shaderID, "dirLight.intensity");
			int p_pos = glGetUniformLocation(shaderID, "dirLightPos");

			glUniform3fv(p_color, 1, glm::value_ptr(color));
			glUniform1fv(p_intensity, 1, &intensity);
			glUniform3fv(p_pos, 1, glm::value_ptr(position));

			glUseProgram(0);
		}
		vec3 color;
		vec3 position;
		float intensity = 0.0f;
		GLuint shaderID;
	};
	class DisplayList {
		public:
			list<shared_ptr<Command>> cmdList;
			void executeOnGPU(list<shared_ptr<Command>> cmdListToExecute);
			void submit();

	};


	DisplayList& addWindowClearCmd(DisplayList* dl, float r, float g, float b, float a);
	DisplayList& addInitMaterialCmd(DisplayList* dl/*, float r, float g, float b, float a*/);
	DisplayList& addDrawCmd(DisplayList* dl, GLuint shaderId, GLuint materialID, GLuint vao, GLsizei size,
		glm::mat4 view, glm::mat4 model);
	DisplayList& addSetPolygonCmd(DisplayList* dl, uint8_t on);
	DisplayList& addSetModelViewProjection(DisplayList* dl, GLuint shaderID, glm::mat4 projection, glm::mat4 model, glm::mat4 view);
	DisplayList& addSetLightDataCmd(DisplayList* dl, glm::vec3 color, glm::vec3 position, float intensity, GLuint shaderID);
}