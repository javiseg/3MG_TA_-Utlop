#pragma once
#include "system.h"
#include "geometry.h"
#include "texture.h"
#include "gameObject.h"
#include <memory>
#include <map>
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

void checkCompileErrors(unsigned int shader, std::string type);
void loadVertexShader(const char* filename, GLuint& vertShader);
void loadFragmentShader(const char* filename, GLuint& fragShader);

namespace Utlop {
	struct RenderToTexture;

	struct RenderCtx {

		vector<shared_ptr<System>> sys;
		vector<shared_ptr<Entity>> entities;
		//shared_ptr<std::vector<shared_ptr<std::vector<Entity>>>> entitiesVector;

		map<int, vector<Component>> cmap;
		map<int, Component> kComponentMap;


		vector<LocalTRComponent> localtrcmp;
		vector<WorldTRComponent> worldtrcmp;
		vector<CameraComponent> cameracmp;
		vector<RenderComponent> rendercmp;
		vector<LightComponent> lightcmp;
		vector<TypeLightComponent> typelighcmp;
		
		vector<Geometry> geometry;
		vector<Material> material;
		vector<GameObject> gObjects;
		vector<Mesh> meshes;

		GLuint vertexShader;
		GLuint fragmentShader;

		RenderComponent cubemap;

		unique_ptr<RenderToTexture> framebuffer;
		unique_ptr<RenderToTexture> shadowframebuffer;

		vector<const char*> obj_str_type;

	};

	struct RenderToTexture {
		GLuint FBOid;
		GLuint FBtexture;
		GLuint RBOid;
		GLuint rectVAO;
		GLuint rectVBO;

		GLuint shaderID;

		glm::mat4 lightProjection;

		float rectangleVertices[24] =
		{
			// Coords    // texCoords
			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f,

			 1.0f,  1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f
		};

		void initFBO(int width, int height) {

			/**/
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

			/**/
			/*
			glCreateFramebuffers(1, &FBOid);

			glCreateTextures(GL_TEXTURE_2D, 1, &FBtexture);
			glTextureParameteri(FBtexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(FBtexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(FBtexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(FBtexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureStorage2D(FBtexture, 1, GL_RGB8, width, height);
			glNamedFramebufferTexture(FBOid, GL_COLOR_ATTACHMENT0, FBtexture, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOid);


			auto fboStatus = glCheckNamedFramebufferStatus(FBOid, GL_FRAMEBUFFER);
			if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
				std::cout << "Framebuffer error: " << fboStatus << "\n";
			}
			*/
		}


		void initShadowFBO(int width, int height) {

			glGenFramebuffers(1, &FBOid);

			// Texture for Shadow Map FBO¡
			glGenTextures(1, &FBtexture);
			glBindTexture(GL_TEXTURE_2D, FBtexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			// Prevents darkness outside the frustrum
			float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

			glBindFramebuffer(GL_FRAMEBUFFER, FBOid);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, FBtexture, 0);
			// Needed since we don't touch the color buffer
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
			//glUseProgram(0);
		}

		void rectangleToGPU() {

			glGenVertexArrays(1, &rectVAO);
			glGenBuffers(1, &rectVBO);
			glBindVertexArray(rectVAO);
			glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		}

		void initShader(const char* vertPath, const char* fragPath) {
			

			
			shaderID = glCreateProgram();

			GLuint fbVertShader;
			GLuint fbFragShader;
			loadVertexShader(vertPath, fbVertShader);
			loadFragmentShader(fragPath, fbFragShader);

			glUseProgram(shaderID);
			glAttachShader(shaderID, fbVertShader);
			glAttachShader(shaderID, fbFragShader);

			glLinkProgram(shaderID);
			checkCompileErrors(shaderID, "PROGRAM");
			checkCompileErrors(shaderID, "LINK");
			glUniform1i(glGetUniformLocation(shaderID, "screenTexture"), 0);
			
		}

		void setLightPerspective(vec3 lightPosition) {

			glm::mat4 orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
			glm::mat4 lightView = glm::lookAt(20.0f * lightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			lightProjection = orthgonalProjection * lightView;

			glUseProgram(shaderID);
			glUniformMatrix4fv(glGetUniformLocation(shaderID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
			glUseProgram(0);
		}
	};


}