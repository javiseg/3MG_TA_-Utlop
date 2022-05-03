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
		kHeritageComp = 8,
		kLightComp = 16,
		kDirectionalLightComp = 32,
		kRenderComp = 64
	};

	enum ComponentPos {
		kLocalTRCompPos = 0,
		kWorldTRCompPos = 1,
		kCameraCompPos = 2,
		kHeritageCompPos = 3,
		kLightCompPos = 4,
		kDirectionalLightCompPos = 5,
		kRenderCompPos = 6
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

	struct HeritageComponent : public Component {
		unsigned int entityID;
		unsigned int parentID;
		mat4 localMatrix;
		mat4 parentMatrix;
	};

	struct LightComponent : public Component {
		vec3 direction;
		vec3 color;
		vec3 position;
		float intensity;
	};

	struct DirectionalLightComponent : public Component {
		vec3 direction;
		vec3 color;
		vec3 position;
		float intensity;
	};

}