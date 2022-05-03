#pragma once
#include "system.h"
#include "geometry.h"
#include "material.h"
#include "gameObject.h"
#include <memory>
#include <map>

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


		vector<Geometry> geometry;
		vector<Material> material;
		vector<GameObject> gObjects;

		unsigned int vertexShader;
		unsigned int fragmentShader;

		RenderComponent cubemap;
	};

}