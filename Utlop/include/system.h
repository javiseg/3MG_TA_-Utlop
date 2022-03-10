#pragma once

#include "entity.h"
#include "component.h"

namespace Utlop {
	struct RenderCtx;

	struct System {
		virtual void exec(Entity& entity, Utlop::RenderCtx* data) {
			
		}
		int id_ = -1;
	};

	struct LocalTRSystem : public System{
		LocalTRSystem() {
			id_ = kLocalTRComp;
		}
		~LocalTRSystem() {}
		void exec(Entity& entity, RenderCtx* data) override;
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
		void exec(Entity& entity, RenderCtx* data) override;
		void init(Entity& entity, RenderCtx* data);
		void update(Entity& entity, RenderCtx* data);
	};
	struct RenderSystem : public System{
		RenderSystem() {
			id_ = kRenderComp;
		}
		~RenderSystem() {}
		void exec(Entity& entity, RenderCtx* data) override;
		void UpdateUniforms(GLuint shaderID);

		void initGeo(Entity& entity, RenderCtx* data);
		bool initMat(Entity& entity, RenderCtx* data);
		void initShader(Entity& entity, RenderCtx* data);
		
	};
	

}