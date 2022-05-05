#pragma once
#include "system.h"
#include "geometry.h"
#include "material.h"
#include "gameObject.h"
#include <memory>
#include <map>

void checkCompileErrors(unsigned int shader, std::string type);
void loadVertexShader(const char* filename, GLuint& vertShader);
void loadFragmentShader(const char* filename, GLuint& fragShader);

namespace Utlop {

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
		vector<DirectionalLightComponent> directionallightcmp;

		vector<Geometry> geometry;
		vector<Material> material;
		vector<GameObject> gObjects;

		GLuint vertexShader;
		GLuint fragmentShader;

		RenderComponent cubemap;
	};

	struct RenderToTexture {
		GLuint FBOid;
		GLuint FBtexture;
		GLuint RBOid;
		GLuint rectVAO;
		GLuint rectVBO;

		GLuint shaderID;

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

			glGenFramebuffers(1, &FBOid);
			glBindFramebuffer(GL_FRAMEBUFFER, FBOid);

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
		}

		void errorCheck() {
			auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
				printf("\nFrameBuffer error: %s\n", fboStatus);
			}
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

		void initShader() {
			

			
			shaderID = glCreateProgram();

			GLuint fbVertShader;
			GLuint fbFragShader;
			loadVertexShader("../UtlopTests/src/shaders/fb_vert.glsl", fbVertShader);
			loadFragmentShader("../UtlopTests/src/shaders/fb_frag.glsl", fbFragShader);

			glUseProgram(shaderID);
			glAttachShader(shaderID, fbVertShader);
			glAttachShader(shaderID, fbFragShader);

			glLinkProgram(shaderID);
			checkCompileErrors(shaderID, "PROGRAM");
			checkCompileErrors(shaderID, "LINK");
			glUniform1i(glGetUniformLocation(shaderID, "screenTexture"), 0);
			glUseProgram(0);
		}

	};

}