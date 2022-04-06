#pragma once
#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"

namespace Utlop {
	using namespace glm;


	enum ComponentID {
		kLocalTRComp = 1,
		kWorldTRComp = 2,
		kCameraComp = 4,
		kRenderComp = 8
	};

	enum ComponentPos {
		kLocalTRCompPos = 0,
		kWorldTRCompPos = 1,
		kCameraCompPos = 2,
		kRenderCompPos = 3
	};


	struct Component {
	};

	struct LocalTRComponent : public Component {
		mat4 model = mat4(1.0f);
		vec3 rotation = vec3(0.0f);
		vec3 scale = vec3(1.0f);
		vec3 position = vec3(0.0f,0.0f,0.0f);
	};
	struct WorldTRComponent : public Component {
		mat4 model;
	};
	struct RenderComponent : public Component {
		// Indice a material, geometría y mat settings?
		vector<GLuint> geo_idx;
		vector<GLuint> material_idx;
		vector<GLuint> materialsettings_idx;
		
		GLuint shaderID_ = 999;
		GLuint vao_ = 999;
		GLuint vbo_ = 999;
		GLuint ebo_ = 999;
		GLuint nvao_ = 999;
		GLuint nbo_ = 999;
		GLuint enbo_ = 999;
		GLuint tvao_ = 999;
		GLuint tbo_ = 999;
		GLuint etbo_ = 999;

	};

	struct CameraComponent : public Component {
		float near_;
		float a_ratio_;
		float fov_;
		float far_;
		float rotation_angle_;
		mat4 view_;
		mat4 projection_;
		float yaw_;
		float pitch_;
		vec3 WorldUp;
		vec3 WorldRight;
		vec3 Up;
		vec3 Right;
		vec3 front_;
		bool hasInit = false;
	};
}