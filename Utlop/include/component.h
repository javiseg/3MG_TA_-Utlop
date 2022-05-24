#pragma once
#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"
#include <vector>

namespace Utlop {
	using namespace glm;
	using namespace std;

	

	enum ComponentID {
		kLocalTRComp = 1,
		kWorldTRComp = 2,
		kCameraComp = 4,
		kHeritageComp = 8,
		kLightComp = 16,
		kTypeLightComp = 32,
		kRenderComp = 64
	};

	enum ComponentPos {
		kLocalTRCompPos = 0,
		kWorldTRCompPos = 1,
		kCameraCompPos = 2,
		kHeritageCompPos = 3,
		kLightCompPos = 4,
		kTypeLightCompPos = 5,
		kRenderCompPos = 6
	};

	enum ComponentConstants {
		kGameObjectsShader = 0
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
		// Indice a material, geometr�a y mat settings?
		vector<GLuint> material_idx;
		vector<GLuint> mesh_idx;

		GLuint shader_idx = 0;
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
		vec3 position_;
	};

	struct HeritageComponent : public Component {
    unsigned int parentID;
	};

	struct LightComponent : public Component {
    int hasLightComponent = 0;
	};

	struct TypeLightComponent : public Component {
		vec3 direction;
		vec3 color;
		vec3 position;
		float intensity;
		int type = 0;
	};

}