#pragma once
#include "system.h"
#include "geometry.h"
#include "material.h"
#include <memory>
#include <map>

namespace Utlop {

	struct RenderCtx {

		vector<shared_ptr<System>> sys;
		vector<shared_ptr<Entity>> entities;
		
		map<int, vector<Component>> cmap;
		map<int, Component> kComponentMap;


		vector<LocalTRComponent> localtrcmp;
		vector<WorldTRComponent> worldtrcmp;
		vector<CameraComponent> cameracmp;
		vector<RenderComponent> rendercmp;


		vector<Geometry> geometry;
		vector<Material> material;

		RenderComponent cubemap;
	};

}