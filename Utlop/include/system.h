#pragma once

#include "entity.h"
#include "component.h"


namespace Utlop {
	struct RenderCtx;
	class DisplayList;

	struct System {
		virtual void preExec(Entity& entity, Utlop::RenderCtx* data) = 0;
		virtual void exec(Entity& entity, Utlop::RenderCtx* data, DisplayList* dl) = 0;
		int id_ = -1;
	};

	struct LocalTRSystem : public System{
		LocalTRSystem() {
			id_ = kLocalTRComp;
		}
		~LocalTRSystem() {}
		void preExec(Entity& entity, Utlop::RenderCtx* data) override;
		void exec(Entity& entity, RenderCtx* data, DisplayList* dl) override;
	};
	struct WorldTRSystem : public System{
		WorldTRSystem() {
			id_ = kWorldTRComp;
		}
		~WorldTRSystem() {}
		void preExec(Entity& entity, Utlop::RenderCtx* data) override {}
		void exec(Entity& entity, RenderCtx* data, DisplayList* dl) override {}
	};
	struct CameraSystem : public System {
		CameraSystem() {
			id_ = kCameraComp;
		}
		~CameraSystem() {}
		void preExec(Entity& entity, Utlop::RenderCtx* data) override;
		void exec(Entity& entity, RenderCtx* data, DisplayList* dl) override;
	};
	struct RenderSystem : public System{
		RenderSystem() {
			id_ = kRenderComp;
		}
		~RenderSystem() {}
		void preExec(Entity& entity, Utlop::RenderCtx* data) override;
		void exec(Entity& entity, RenderCtx* data, DisplayList* dl) override;
		void UpdateUniforms(GLuint shaderID);

		void initGeo(Entity& entity, RenderCtx* data, const char* path);
		bool initMat(Entity& entity, RenderCtx* data, const char* path);
		void initShader(Entity& entity, RenderCtx* data);
		
	};

	struct HeritageSystem : public System {
		HeritageSystem() {
			id_ = kHeritageComp;
		}
		~HeritageSystem() {}
		void preExec(Entity& entity, Utlop::RenderCtx* data) override;
		void exec(Entity& entity, RenderCtx* data, DisplayList* dl) override;

	};
	
	struct LightSystem : public System {
		LightSystem() {
			id_ = kLightComp;
		}
		~LightSystem() {}
		void preExec(Entity& entity, Utlop::RenderCtx* data) override;
		void exec(Entity& entity, RenderCtx* data, DisplayList* dl) override;
	};

	struct DirectionalLightSystem : public System {
		DirectionalLightSystem() {
			id_ = kDirectionalLightComp;
		}
		~DirectionalLightSystem() {}
		void preExec(Entity& entity, Utlop::RenderCtx* data) override;
		void exec(Entity& entity, RenderCtx* data, DisplayList* dl) override;
	};
}