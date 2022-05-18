#include "system.h"
#include "data.h"
#include "mesh.h"
#include "texture.h"
#include "displaylist.h"
#include "core.h"
#include "GLFW/glfw3.h"

void Utlop::LocalTRSystem::preExec(Entity& entity, Utlop::RenderCtx* data)
{
  data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model = mat4(1.0f);

  data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
    translate(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].position);

  data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
    glm::scale(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].scale);

  data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
    rotate(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, radians(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].rotation.x), vec3(1.0f, 0.0f, 0.0f));

  data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
    rotate(data->localtrcmp[entity.cmp_indx_[0]].model, radians(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].rotation.y), vec3(0.0f, 1.0f, 0.0f));

  data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
    rotate(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, radians(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].rotation.z), vec3(0.0f, 0.0f, 1.0f));
}

void Utlop::LocalTRSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{
  /*
	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model = mat4(1.0f);

	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
		translate(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].position);

	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
		glm::scale(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].scale);

	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
		rotate(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, radians(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].rotation.x), vec3(1.0f, 0.0f, 0.0f));

	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
		rotate(data->localtrcmp[entity.cmp_indx_[0]].model, radians(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].rotation.y), vec3(0.0f, 1.0f, 0.0f));

	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
		rotate(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, radians(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].rotation.z), vec3(0.0f, 0.0f, 1.0f));
  */
}


void Utlop::CameraSystem::preExec(Entity& entity, Utlop::RenderCtx* data)
{
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].near_ = 0.1f;
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].a_ratio_ = 1380.0f / 780.0f;
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].fov_ = glm::radians(45.0f);
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].far_ = 300.0f;
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].rotation_angle_ = 0.0f;
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].projection_ =
		glm::perspective(data->cameracmp[entity.cmp_indx_[kCameraCompPos]].fov_, data->cameracmp[entity.cmp_indx_[kCameraCompPos]].a_ratio_,
			data->cameracmp[entity.cmp_indx_[kCameraCompPos]].near_, data->cameracmp[entity.cmp_indx_[kCameraCompPos]].far_);
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].yaw_ = -90.0f;
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].pitch_ = 0.0f;
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].WorldUp = vec3(0.0f, 1.0f, 0.0f);
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].WorldRight = vec3(1.0f, 0.0f, 0.0f);
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].position_ = vec3(0.0f);
}

void Utlop::CameraSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].position_ = data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].position;
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].front_.x =
		cos(glm::radians(data->cameracmp[entity.cmp_indx_[kCameraCompPos]].yaw_)) * cos(glm::radians(data->cameracmp[entity.cmp_indx_[kCameraCompPos]].pitch_));
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].front_.y =
		sin(glm::radians(data->cameracmp[entity.cmp_indx_[kCameraCompPos]].pitch_));
	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].front_.z =
		sin(glm::radians(data->cameracmp[entity.cmp_indx_[kCameraCompPos]].yaw_)) * cos(glm::radians(data->cameracmp[entity.cmp_indx_[kCameraCompPos]].pitch_));

	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].front_ = normalize(data->cameracmp[entity.cmp_indx_[kCameraCompPos]].front_);

	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].Right =
		normalize(glm::cross(data->cameracmp[entity.cmp_indx_[kCameraCompPos]].front_, data->cameracmp[entity.cmp_indx_[kCameraCompPos]].WorldUp));

	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].Up =
		normalize(cross(data->cameracmp[entity.cmp_indx_[kCameraCompPos]].Right, data->cameracmp[entity.cmp_indx_[kCameraCompPos]].front_));

	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].view_ = glm::lookAt(data->cameracmp[entity.cmp_indx_[kCameraCompPos]].position_,
		data->cameracmp[entity.cmp_indx_[kCameraCompPos]].position_ + data->cameracmp[entity.cmp_indx_[kCameraCompPos]].front_, data->cameracmp[entity.cmp_indx_[kCameraCompPos]].Up);
}


void Utlop::RenderSystem::preExec(Entity& entity, Utlop::RenderCtx* data)
{
	if (entity.cmp_indx_[kLocalTRCompPos] != -1) {
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shader_idx = kGameObjectsShader;
	}
	//entity.cmp_indx_[kRenderCompPos] = 0;
	if (entity.cmp_indx_[kTypeLightCompPos] == -1) {
		initialMesh(entity, data, "../UtlopTests/src/obj/robot/robot.obj");
	}
	else {
		initialMesh(entity, data, "../UtlopTests/src/obj/lightcube.obj");
	}
}

void Utlop::RenderSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{
	addSetModelViewProjection(dl,data->shaders[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shader_idx].id, 
		data->cameracmp[0].projection_, data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model,
		data->cameracmp[0].view_);
	
	addDrawMeshCmd(dl, data->meshes[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].mesh_idx[0]], 
		data->shaders[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shader_idx].id,	data->cameracmp[0], 
		data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]]);
}


void Utlop::RenderSystem::initialMesh(Entity& entity, RenderCtx* data, const char* path)
{
	if (data->rendercmp[entity.cmp_indx_[kRenderCompPos]].mesh_idx.empty()) {
    int mesh_index = -1;
    for (unsigned int i = 0; i < data->meshes.size(); i++) {
      if (data->meshes[i].path._Equal(path)) {
        mesh_index = i;
      }
    }
    if (mesh_index != -1) {
      data->rendercmp[entity.cmp_indx_[kRenderCompPos]].mesh_idx.push_back(mesh_index);
    }
  }
}

void Utlop::HeritageSystem::preExec(Entity& entity, Utlop::RenderCtx* data)
{
}

void Utlop::HeritageSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{
}

void Utlop::LightSystem::preExec(Entity& entity, Utlop::RenderCtx* data)
{
	data->lightcmp[entity.cmp_indx_[kLightCompPos]].color = vec3(1.0f, 1.0f, 1.0f);
	data->lightcmp[entity.cmp_indx_[kLightCompPos]].direction = vec3(1.0f, 1.0f, 1.0f);
	data->lightcmp[entity.cmp_indx_[kLightCompPos]].intensity = 2.0f;
	data->lightcmp[entity.cmp_indx_[kLightCompPos]].position = vec3(0.0f, 0.0f, 20.0f);
}

void Utlop::LightSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{
	if ((entity.componentsID_ & kRenderComp) == kRenderComp) {
		
		for (int i = 0; i < data->typelighcmp.size(); i++) {
			addSetLightDataCmd(dl, data->typelighcmp[i].color, data->typelighcmp[i].position,
				data->typelighcmp[i].intensity, data->shaders[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shader_idx].id,
				data->localtrcmp[0].position, data->typelighcmp[i].direction, data->typelighcmp[i].type);
		}
	}
}

void Utlop::TypeLightSystem::preExec(Entity& entity, Utlop::RenderCtx* data)
{
	if (entity.cmp_indx_[kLocalTRCompPos] != -1) {
		data->typelighcmp[entity.cmp_indx_[kTypeLightCompPos]].color = vec3(1.0f, 1.0f, 1.0f);
		data->typelighcmp[entity.cmp_indx_[kTypeLightCompPos]].direction = vec3(0.0f, -1.0f, 0.0f);
		data->typelighcmp[entity.cmp_indx_[kTypeLightCompPos]].intensity = 2.0f;
		data->typelighcmp[entity.cmp_indx_[kTypeLightCompPos]].position = data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].position;
	}
	else {
		printf("\nERROR: Point light entity without transform\n");
	}
}

void Utlop::TypeLightSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{
	if (entity.cmp_indx_[kLocalTRCompPos] != -1) {
		data->typelighcmp[entity.cmp_indx_[kTypeLightCompPos]].position = data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].position;
		data->typelighcmp[entity.cmp_indx_[kTypeLightCompPos]].direction = data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].position;
	}
  data->shadowframebuffer->setLightPerspective(normalize(data->localtrcmp[1].position + vec3(0.001f, 0.001f, 0.001f)), &data->shaders,
    data->typelighcmp[entity.cmp_indx_[kTypeLightCompPos]].type);
}