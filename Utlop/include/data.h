#pragma once
#include "system.h"
#include "mesh.h"
#include "texture.h"
#include "gameObject.h"
#include <memory>
#include <map>
#include "mesh.h"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"
#include <iostream>

namespace Utlop {
	struct RenderToTexture;

	struct RenderCtx {

		vector<shared_ptr<System>> sys;
		vector<shared_ptr<Entity>> entities;

		vector<LocalTRComponent> localtrcmp;
		vector<CameraComponent> cameracmp;
		vector<RenderComponent> rendercmp;
		vector<LightComponent> lightcmp;
		vector<TypeLightComponent> typelighcmp;
    vector<HeritageComponent> heritagecmp;
		
		vector<Mesh> meshes;
		vector<Material> materials;

		GLuint vertexShader;
		GLuint fragmentShader;

		RenderComponent cubemap;

		vector<Shader> shaders;

		unique_ptr<RenderToTexture> framebuffer;
		unique_ptr<RenderToTexture> shadowframebuffer;

		vector<const char*> obj_str_type;
		vector<const char*> mat_str_type;

    bool imguimoUsing = false;
	};

	struct RenderToTexture {
		GLuint FBOid;
		GLuint FBtexture;
		GLuint RBOid;
		GLuint rectVAO;
		GLuint rectVBO;
		GLuint rectEBO;
    GLint type = 0;

		int width;
		int height;

    GLint indices;

		GLuint shader_idx;

		glm::mat4 lightProjection;

    

		void initFBO(int width_, int height_) {

			/**/
			width = width_;
			height = height_;
			glGenFramebuffers(1, &FBOid);
			glBindFramebuffer(GL_FRAMEBUFFER, FBOid);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
				printf("\nFramebuffer binded\n");
			}
			glGenTextures(1, &FBtexture);
			glBindTexture(GL_TEXTURE_2D, FBtexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBtexture, 0);

			glGenRenderbuffers(1, &RBOid);
			glBindRenderbuffer(GL_RENDERBUFFER, RBOid);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOid);
			glUseProgram(0);

		}


		void initShadowFBO(int width_, int height_) {

			width = width_;
			height = height_;
			glGenFramebuffers(1, &FBOid);
			
			glGenTextures(1, &FBtexture);
			glBindTexture(GL_TEXTURE_2D, FBtexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		
      float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

			glBindFramebuffer(GL_FRAMEBUFFER, FBOid);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, FBtexture, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		void errorCheck() {
			glUseProgram(FBOid);
			auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
				printf("\nFrameBuffer error: %s\n", fboStatus);
			}
			glUseProgram(0);
		}

		void rectangleToGPU() {


      const GLfloat vertices_[20] =
      {
        -1.0f, -1.0f , 0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f , 0.0f, 0.0f, 1.0f,
         1.0f,  1.0f , 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f , 0.0f, 1.0f, 0.0f,
      };

      const GLuint indices_[6] =
      {
        0, 2, 1,
        0, 3, 2
      };

      indices = sizeof(indices_) / sizeof(indices_[0]);

      glCreateVertexArrays(1, &rectVAO);
      glCreateBuffers(1, &rectVBO);
      glCreateBuffers(1, &rectEBO);
      glNamedBufferData(rectVBO, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
      glNamedBufferData(rectEBO, sizeof(indices_),indices_, GL_STATIC_DRAW);

      glEnableVertexArrayAttrib(rectVAO, 0);
      glVertexArrayAttribBinding(rectVAO, 0, 0);
      glVertexArrayAttribFormat(rectVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);

      glEnableVertexArrayAttrib(rectVAO, 1);
      glVertexArrayAttribBinding(rectVAO, 1, 0);
      glVertexArrayAttribFormat(rectVAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

      glVertexArrayVertexBuffer(rectVAO, 0, rectVBO, 0, 5 * sizeof(float));

      glVertexArrayElementBuffer(rectVAO, rectEBO);
		}

		void initShader(GLuint shader_index) {
			
			shader_idx = shader_index;

		}

		void setLightPerspective(TypeLightComponent light, vector<Shader>* shaders) {
      
      glm::mat4 projection = mat4(1.0f);// = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 500.0f);
      glm::mat4 lightView = mat4(1.0f); // = glm::lookAt(40.0f * lightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      light.position += vec3(0.001f, 0.001f, 0.001f);
      light.direction += vec3(0.001f, 0.001f, 0.001f);
      
      //glm::mat4 orthgonalProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 500.0f);
      //glm::mat4 lightView = mat4(1.0f); 
      switch (light.type) {
        // PointLight
        case 0: {
            projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 500.0f);
          break;
        };
        // DirectionalLight
        case 1: {
            light.position = normalize(light.position);
            projection = glm::ortho(-200.0f, 200.0f, -100.0f, 100.0f, 0.1f, 500.0f);
            lightView = glm::lookAt(40.0f * light.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
          break;
        };
        // SpotLight
        case 2: {
            light.position -=  vec3(0.0f, 3.0f, 0.0f);
            projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 200.0f);
            lightView = glm::lookAt(light.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
          break;
        };
      }
      //lightPosition = vec3(0.0f, 1.0f, 0.0f);
      //glm::vec3 lightPos = glm::vec3(1.5f, 0.5f, 0.5f);
      lightProjection = projection * lightView;

      glUseProgram(shaders->at(shader_idx).id);
      glUniformMatrix4fv(glGetUniformLocation(shaders->at(shader_idx).id, "lightprojection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
      glUseProgram(0);
		}
	};


}