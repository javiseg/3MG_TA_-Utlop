#include "system.h"
#include "data.h"
#include "geometry.h"
#include "material.h"
#include "utility.h"
#include "displaylist.h"

void Utlop::LocalTRSystem::preExec(Entity& entity, Utlop::RenderCtx* data)
{
}

void Utlop::LocalTRSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{
	UpdateModel(entity, data);
	//data->localtrcmp[entity.cmp_indx_[0]].position += vec3(1.0f, 0.0f, 0.0f);
}

void Utlop::LocalTRSystem::UpdateModel(Entity& entity, RenderCtx* data)
{
	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model = mat4(1.0f);

	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
		translate(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, vec3(0.0f));

	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
		rotate(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, radians(data->localtrcmp[entity.cmp_indx_[0]].rotation.x), vec3(1.0f, 0.0f, 0.0f));

	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
		rotate(data->localtrcmp[entity.cmp_indx_[0]].model, radians(data->localtrcmp[entity.cmp_indx_[0]].rotation.y), vec3(0.0f, 1.0f, 0.0f));

	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
		rotate(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, radians(data->localtrcmp[entity.cmp_indx_[0]].rotation.z), vec3(0.0f, 0.0f, 1.0f));

	
	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
		translate(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, data->localtrcmp[entity.cmp_indx_[0]].position);

	data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model =
		glm::scale(data->localtrcmp[entity.cmp_indx_[kLocalTRCompPos]].model, data->localtrcmp[entity.cmp_indx_[0]].scale);


}

void Utlop::CameraSystem::preExec(Entity& entity, Utlop::RenderCtx* data)
{
	init(entity, data);
}

void Utlop::CameraSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{
	update(entity, data);
}

void Utlop::CameraSystem::init(Entity& entity, RenderCtx* data)
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

void Utlop::CameraSystem::update(Entity& entity, RenderCtx* data)
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
	initGeo(entity, data, "../UtlopTests/src/obj/robot.obj");
	initMat(entity, data, "../UtlopTests/src/textures/texture.jpg");
	initMat(entity, data, "../UtlopTests/src/textures/default.png");
	
	setMat4fv(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_,
		data->cameracmp[0].projection_, "ProjectionMatrix");
}

void Utlop::RenderSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{
	setMat4fv(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_,
		data->localtrcmp[entity.cmp_indx_[0]].model,
		"ModelMatrix");

	setMat4fv(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_,
		data->cameracmp[0].view_, "ViewMatrix");

	addDrawCmd(dl, data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_,
		data->material[0].diff_, data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_,
		data->geometry[0].verticesIndices_.size());


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
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_ = data->rendercmp[0].vao_;
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vbo_ = data->rendercmp[0].vbo_;
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].ebo_ = data->rendercmp[0].ebo_;
	}
	
	//glUseProgram(0);
}

void Utlop::RenderSystem::initMat(Entity& entity, RenderCtx* data, const char* path)
{
	stbi_set_flip_vertically_on_load(true);
	

	Material tmpText;
	//Debug:
	tmpText.path_ = path;

	for (unsigned int i = 0; i < data->material.size(); i++) {
		if (data->material[i].path_._Equal(tmpText.path_)) {
			data->rendercmp[entity.cmp_indx_[kRenderCompPos]].material_idx.push_back(i);
		}
	}
}

void Utlop::RenderSystem::initShader(Entity& entity, RenderCtx* data)
{
	data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_ = glCreateProgram();

	glUseProgram(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_);
	glAttachShader(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, data->vertexShader);
	glAttachShader(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, data->fragmentShader);
	
	glLinkProgram(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_);
	glUseProgram(0);
	checkCompileErrors(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, "PROGRAM");
	checkCompileErrors(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, "LINK");
}