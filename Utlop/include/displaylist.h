#pragma once
#include <list>
#include <memory>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/mat4x4.hpp"
#include "mesh.h"
#include "data.h"

namespace Utlop {

	using namespace std;
	using namespace glm;

	struct Command {
		virtual void executeOnGPU() = 0;
	};
	struct WindowClearCmd : public Command {
		void executeOnGPU() override {
			glClearColor(r, g, b, a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		}
		float r, g, b, a;
	};
  struct ViewPortCmd : public Command {
    void executeOnGPU() override {
      glViewport(x, y, width, height);
    }
    GLint x, y;
    GLsizei width, height;
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

	struct DrawMeshCmd : public Command {
		void executeOnGPU() override {
			m.Draw(shader, cameracmp, hasLightComponent, mat);
		}
		Mesh m;
    Material mat;
		GLuint shader;
		CameraComponent cameracmp;
		int hasLightComponent;
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
	
  struct SetLightDataCmd : public Command {
		void executeOnGPU() override {
			glUseProgram(shaderID);
      
      int p_dir = -1;
      int p_color = -1;
      int p_intensity = -1;
      int p_pos = -1;
      int t_pos = -1;
      int c_pos = -1;
      
      switch (type) {
        case 0: {
          p_dir = glGetUniformLocation(shaderID, "pointLight.direction");
          p_color = glGetUniformLocation(shaderID, "pointLight.color");
          p_intensity = glGetUniformLocation(shaderID, "pointLight.intensity");
          p_pos = glGetUniformLocation(shaderID, "pointLight.dirLightPos");
        }; break;
        case 1: {
          p_dir = glGetUniformLocation(shaderID, "directionalLight.direction");
          p_color = glGetUniformLocation(shaderID, "directionalLight.color");
          p_intensity = glGetUniformLocation(shaderID, "directionalLight.intensity");
          p_pos = glGetUniformLocation(shaderID, "directionalLight.dirLightPos");
        }; break;
        case 2: { //SpotLight
          p_dir = glGetUniformLocation(shaderID, "spotLight.direction");
          p_color = glGetUniformLocation(shaderID, "spotLight.color");
          p_intensity = glGetUniformLocation(shaderID, "spotLight.intensity");
          p_pos = glGetUniformLocation(shaderID, "spotLight.LightPos");
        }; break;
        }
      t_pos = glGetUniformLocation(shaderID, "l_type");
      c_pos = glGetUniformLocation(shaderID, "camPos");

      glUniform1iv(t_pos, 1, &type);
      glUniform3fv(p_color, 1, glm::value_ptr(color));
      glUniform1fv(p_intensity, 1, &intensity);
      glUniform3fv(p_pos, 1, glm::value_ptr(position));
      glUniform3fv(c_pos, 1, glm::value_ptr(camPosition));
      glUniform3fv(p_dir, 1, glm::value_ptr(direction));
      

			glUseProgram(0);
		}
		vec3 color;
		vec3 position;
		float intensity = 0.0f;
		GLuint shaderID;
		vec3 direction;
		vec3 camPosition;
		int type;
	};

	struct EnableDepthCmd : public Command {
		void executeOnGPU() override {
			glEnable(GL_DEPTH_TEST);
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
      glBindTextureUnit(0, fbTexture);
      glBindTextureUnit(1, 3);
      glUniform1i(glGetUniformLocation(shaderID, "screenTexture"), 0);
      glUniform1i(glGetUniformLocation(shaderID, "silhoutteTexture"), 1);
      glUniform1i(glGetUniformLocation(shaderID, "shadowMap"), 2);



      glUniform1i(glGetUniformLocation(shaderID, "postProcessType"), type);
      glBindVertexArray(rectVAO);
      glDrawElements(GL_TRIANGLES, indicessize, GL_UNSIGNED_INT, 0);
      glUseProgram(0);
		}
		GLuint shaderID;
		GLuint rectVAO;
		GLuint fbTexture;
		GLuint fbo;
    GLuint indicessize = 6;
    GLint type = 0;
	};

	struct BindFramebufferCmd : public Command {
		void executeOnGPU() override {
			glBindFramebuffer(GL_FRAMEBUFFER, fboID);
		}
		GLuint fboID;
	};
  struct ClearDepthBufferCmd : public Command {
    void executeOnGPU() override {
      glClear(GL_DEPTH_BUFFER_BIT);
    }
  };

	struct SkyboxCmd : public Command {
		void executeOnGPU() override {
			glUseProgram(shaderID);
			glDepthFunc(GL_ALWAYS);
			glDepthMask(GL_FALSE);
			
      view = glm::mat4(glm::mat3(glm::lookAt(position, position
				+ front, Up)));
			glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			glBindVertexArray(vao);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

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

	struct ShadowMapCmd : public Command {
		void executeOnGPU() override {
			
      glUseProgram(shaderID);
      glUniformMatrix4fv(glGetUniformLocation(shaderID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
      glActiveTexture(GL_TEXTURE0 + activeTexture);
      glBindTexture(GL_TEXTURE_2D, FBtexture);
      glUniform1i(glGetUniformLocation(shaderID, "shadowMap"), 2);
      glUseProgram(0);

		}
    GLuint shaderID;
    mat4 lightProjection;
    GLuint activeTexture;
    GLuint FBtexture;
	};
	struct SilhoutteCmd : public Command {
		void executeOnGPU() override {
			
      glUseProgram(shaderID);
      glUniformMatrix4fv(glGetUniformLocation(shaderID, "colorSilhoutte"), 1, GL_FALSE, glm::value_ptr(color));
      glActiveTexture(GL_TEXTURE0 + activeTexture);
      glBindTexture(GL_TEXTURE_2D, FBtexture);
      glUniform1i(glGetUniformLocation(shaderID, "silhoutteMap"), 4);
      glUseProgram(0);

		}
    GLuint activeTexture;
    GLuint FBtexture;
    vec3 color;
    GLuint shaderID;
    GLuint fbid;
	};
	class DisplayList {
		public:
			list<shared_ptr<Command>> cmdList;
			void executeOnGPU();

	};


	DisplayList& addWindowClearCmd(DisplayList* dl, float r, float g, float b, float a);
  DisplayList& addViewPortCmd(DisplayList* dl, GLint x, GLint y, GLsizei width, GLsizei height);
  DisplayList& addDrawCmd(DisplayList* dl, GLuint shaderId, GLuint materialID, GLuint vao, GLsizei size,
		glm::mat4 view, glm::mat4 model);
	DisplayList& addDrawMeshCmd(DisplayList* dl, Utlop::Mesh m, GLuint shader, Utlop::CameraComponent cameracmp, Utlop::LocalTRComponent localcmp,int hasLightComponent, Material mat);
	DisplayList& addSetPolygonCmd(DisplayList* dl, uint8_t on);
	DisplayList& addSetModelViewProjection(DisplayList* dl, GLuint shaderID, glm::mat4 projection, glm::mat4 model, glm::mat4 view);
	DisplayList& addSetLightDataCmd(DisplayList* dl, glm::vec3 color, glm::vec3 position, float intensity, GLuint shaderID, vec3 camPosition, vec3 direction, int type);
	DisplayList& addEnableDepthTest(DisplayList* dl);
	DisplayList& addDisableDepthTest(DisplayList* dl);
  DisplayList& addClearDepthBufferCmd(DisplayList* dl);
  DisplayList& addDoFramebuffer(DisplayList* dl, GLuint shaderID, GLuint rectVAO, GLuint texture, GLuint fbo, GLuint indicessize, GLint fb_type);
	DisplayList& addBindFramebuffer(DisplayList* dl, GLuint fboID);
	DisplayList& addShadowFrameBufferCmd(DisplayList* dl, GLuint shaderID, mat4 lightProjection, GLuint activeTexture, GLuint FBtexture);
	DisplayList& addDrawSkybox(DisplayList* dl, GLuint shaderID,
		glm::vec3 position, glm::vec3 front, glm::vec3 Up, glm::mat4 projection,
		glm::mat4 view, GLuint vao, GLuint texture);
  DisplayList& addSilhoutteCmd(DisplayList* dl, GLuint shaderID, vec3 color, GLuint fboid, GLuint activeTexture, GLuint FBtexture);
}