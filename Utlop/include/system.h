#pragma once

#include "entity.h"
#include "component.h"


namespace Utlop {
	struct RenderCtx;
	class DisplayList;

	struct System {
		virtual void preExec(Entity& entity, Utlop::RenderCtx* data) {

		}
		virtual void exec(Entity& entity, Utlop::RenderCtx* data, DisplayList* dl) {
			
		}
		int id_ = -1;
	};

	struct LocalTRSystem : public System{
		LocalTRSystem() {
			id_ = kLocalTRComp;
		}
		~LocalTRSystem() {}
		void preExec(Entity& entity, Utlop::RenderCtx* data) override;
		void exec(Entity& entity, RenderCtx* data, DisplayList* dl) override;
		void UpdateModel(Entity& entity, RenderCtx* data);
	};
	struct WorldTRSystem : public System{
		WorldTRSystem() {
			id_ = kWorldTRComp;
		}
		~WorldTRSystem() {}
	};
	struct CameraSystem : public System {
		CameraSystem() {
			id_ = kCameraComp;
		}
		~CameraSystem() {}
		void preExec(Entity& entity, Utlop::RenderCtx* data) override;
		void exec(Entity& entity, RenderCtx* data, DisplayList* dl) override;
		void init(Entity& entity, RenderCtx* data);
		void update(Entity& entity, RenderCtx* data);
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
	

}