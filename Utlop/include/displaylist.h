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
			int p_pos = glGetUniformLocation(shaderID, "dirLight.dirLightPos");
			int c_pos = glGetUniformLocation(shaderID, "camPos");

			glUniform3fv(p_color, 1, glm::value_ptr(color));
			glUniform1fv(p_intensity, 1, &intensity);
			glUniform3fv(p_pos, 1, glm::value_ptr(position));
			glUniform3fv(c_pos, 1, glm::value_ptr(camPosition));

			glUseProgram(0);
		}
		vec3 color;
		vec3 position;
		float intensity = 0.0f;
		GLuint shaderID;
		vec3 camPosition;
	};

	struct EnableDepthCmd : public Command {
		void executeOnGPU() override {
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}
	};

	struct DisableDepthCmd : public Command {
		void executeOnGPU() override {
			glDisable(GL_DEPTH_TEST);
		}
	};

	struct DoFrameBufferCmd : public Command {
		void executeOnGPU() override {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glUseProgram(shaderID);
			glBindVertexArray(rectVAO);
			glBindTexture(GL_TEXTURE_2D, fbTexture);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		GLuint shaderID;
		GLuint rectVAO;
		GLuint fbTexture;

	};

	struct BindFramebufferCmd : public Command {
		void executeOnGPU() override {
			glBindFramebuffer(GL_FRAMEBUFFER, fboID);
		}
		GLuint fboID;
	};

	struct SkyboxCmd : public Command {
		void executeOnGPU() override {
			glUseProgram(shaderID);
			glDepthFunc(GL_ALWAYS);
			glDepthMask(GL_FALSE);
			// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
			// The last row and column affect the translation of the skybox (which we don't want to affect)
			view = glm::mat4(glm::mat3(glm::lookAt(position, position
				+ front, Up)));
			glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
			// where an object is present (a depth of 1.0f will always fail against any object's depth value)
			glBindVertexArray(vao);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// Switch back to the normal depth function
			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);
			glUseProgram(0);
		}
		GLuint shaderID;
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 Up;
		glm::mat4 projection;
		glm::mat4 view;
		GLuint vao;
		GLuint texture;
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
	DisplayList& addSetLightDataCmd(DisplayList* dl, glm::vec3 color, glm::vec3 position, float intensity, GLuint shaderID, vec3 camPosition);
	DisplayList& addEnableDepthTest(DisplayList* dl);
	DisplayList& addDisableDepthTest(DisplayList* dl);
	DisplayList& addDoFramebuffer(DisplayList* dl, GLuint shaderID, GLuint rectVAO, GLuint texture);
	DisplayList& addBindFramebuffer(DisplayList* dl, GLuint fboID);
	DisplayList& addDrawSkybox(DisplayList* dl, GLuint shaderID,
		glm::vec3 position, glm::vec3 front, glm::vec3 Up, glm::mat4 projection,
		glm::mat4 view, GLuint vao, GLuint texture);
}