#include "system.h"
#include "data.h"
#include "geometry.h"
#include "material.h"
#include "displaylist.h"
#include "core.h"
#include "GLFW/glfw3.h"

void Utlop::LocalTRSystem::preExec(Entity& entity, Utlop::RenderCtx* data)
{
}

void Utlop::LocalTRSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
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
}

void Utlop::CameraSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{
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

	data->cameracmp[entity.cmp_indx_[kCameraCompPos]].view_ = glm::lookAt(data->localtrcmp[entity.cmp_indx_[0]].position,
		data->localtrcmp[entity.cmp_indx_[0]].position + data->cameracmp[entity.cmp_indx_[kCameraCompPos]].front_, data->cameracmp[entity.cmp_indx_[kCameraCompPos]].Up);
}


void Utlop::RenderSystem::preExec(Entity& entity, Utlop::RenderCtx* data)
{
	initShader(entity, data);
	if (entity.cmp_indx_[kDirectionalLightCompPos] == -1) {
		initGeo(entity, data, "../UtlopTests/src/obj/robot/robot.obj");
		initMat(entity, data, "../UtlopTests/src/obj/robot/diffuse.jpg");
	}else{
		initGeo(entity, data, "../UtlopTests/src/obj/cube.obj");
		initMat(entity, data, "../UtlopTests/src/textures/white.png");
	}
	//initMat(entity, data, "../UtlopTests/src/textures/default.png");
	
}

void Utlop::RenderSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{

	addSetModelViewProjection(dl, data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, 
		data->cameracmp[0].projection_, data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model,
		data->cameracmp[0].view_);
	
	for (int i = 0; i < data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.size(); i++) {
		for (int j = 0; j < data->rendercmp[entity.cmp_indx_[kRenderCompPos]].material_idx.size(); j++) {

			addDrawCmd(dl, data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_,
				data->material[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].material_idx[j]].diff_, 
				data->geometry[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx[i]].vao_,
				data->geometry[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx[i]].totalIndices_.size(),
				data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, data->cameracmp[0].view_);
		}
	}

}


void Utlop::RenderSystem::UpdateUniforms(GLuint shaderID)
{
	
}

void Utlop::RenderSystem::initGeo(Entity& entity, RenderCtx* data, const char* path)
{
	int geo_index = -1;
	for (unsigned int i = 0; i < data->geometry.size(); i++) {
		if (data->geometry[i].path._Equal(path)) {
			geo_index = i;
		}
	}
	if (geo_index != -1) {
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.push_back(geo_index);
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_ = data->geometry[geo_index].vao_;
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vbo_ = data->geometry[geo_index].vbo_;
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].ebo_ = data->geometry[geo_index].ebo_;
	}
}

bool Utlop::RenderSystem::initMat(Entity& entity, RenderCtx* data, const char* path)
{
	Material tmpText;
	//Debug:
	tmpText.path_ = path;

	for (unsigned int i = 0; i < data->material.size(); i++) {
		if (data->material[i].path_._Equal(tmpText.path_)) {
			data->rendercmp[entity.cmp_indx_[kRenderCompPos]].material_idx.push_back(i);
			return true;
		}
	}


	return false;
}

void Utlop::RenderSystem::initShader(Entity& entity, RenderCtx* data)
{
	data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_ = glCreateProgram();

	glUseProgram(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_);
	glAttachShader(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, data->vertexShader);
	glAttachShader(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, data->fragmentShader);
	
	glLinkProgram(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_);

	checkCompileErrors(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, "PROGRAM");
	checkCompileErrors(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, "LINK");

	glUseProgram(0);
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
	if (entity.cmp_indx_[kRenderCompPos] != -1) {
		
			for (int i = 0; i < data->directionallightcmp.size(); i++) {
				addSetLightDataCmd(dl, data->directionallightcmp[i].color, data->directionallightcmp[i].position,
					data->directionallightcmp[i].intensity, data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_);
			}
	}
}

void Utlop::DirectionalLightSystem::preExec(Entity& entity, Utlop::RenderCtx* data)
{
	if (entity.cmp_indx_[kLocalTRCompPos] != -1) {
		data->directionallightcmp[entity.cmp_indx_[kDirectionalLightCompPos]].color = vec3(1.0f, 1.0f, 1.0f);
		data->directionallightcmp[entity.cmp_indx_[kDirectionalLightCompPos]].direction = vec3(1.0f, 1.0f, 1.0f);
		data->directionallightcmp[entity.cmp_indx_[kDirectionalLightCompPos]].intensity = 2.0f;
		data->directionallightcmp[entity.cmp_indx_[kDirectionalLightCompPos]].position = data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].position;
	}
	else {
		printf("\nERROR: Directional light entity without transform\n");
	}
}

void Utlop::DirectionalLightSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{
	if (entity.cmp_indx_[kLocalTRCompPos] != -1) {
		data->directionallightcmp[entity.cmp_indx_[kDirectionalLightCompPos]].position = data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].position;
	}
}
