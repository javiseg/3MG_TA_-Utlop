#include "system.h"
#include "data.h"
#include "geometry.h"
#include "material.h"
#include "utility.h"

void Utlop::LocalTRSystem::preExec(Entity& entity, Utlop::RenderCtx* data)
{
}

void Utlop::LocalTRSystem::exec(Entity& entity, RenderCtx* data)
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

void Utlop::CameraSystem::exec(Entity& entity, RenderCtx* data)
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
	if (data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.empty())
		initGeo(entity, data);
	if (data->rendercmp[entity.cmp_indx_[kRenderCompPos]].material_idx.empty()) {
		initMat(entity, data, "../UtlopTests/src/textures/texture.jpg");
		initMat(entity, data, "../UtlopTests/src/textures/default.png");
	}
	if (data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_ == 999)
		initShader(entity, data);

	setMat4fv(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_,
		data->cameracmp[0].projection_, "ProjectionMatrix");
}

void Utlop::RenderSystem::exec(Entity& entity, RenderCtx* data)
{
	setMat4fv(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_,
		data->localtrcmp[entity.cmp_indx_[0]].model,
		"ModelMatrix");

	setMat4fv(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_,
		data->cameracmp[0].view_, "ViewMatrix");

	glUseProgram(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_);
	
	glBindTextureUnit(0, data->material[0].diff_);

	glBindVertexArray(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_);

	glDrawElements(GL_TRIANGLES, data->geometry[0].verticesIndices_.size(), GL_UNSIGNED_INT, 0);

	glUseProgram(0);


}


void Utlop::RenderSystem::UpdateUniforms(GLuint shaderID)
{
	
}

void Utlop::RenderSystem::initGeo(Entity& entity, RenderCtx* data)
{
	int geo_index = -1;
	for (unsigned int i = 0; i < data->geometry.size(); i++) {
		if (data->geometry[i].path._Equal("../UtlopTests/src/obj/cube.obj")) {
			geo_index = i;
		}
	}
	if (geo_index != -1) {
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.push_back(geo_index);
	}
	else {
		Geometry geo;
		loadOBJ("../UtlopTests/src/obj/cube.obj", geo);
		data->geometry.push_back(geo);
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.push_back(data->geometry.size() - 1);
	}
	glCreateVertexArrays(1, &data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_);
	glCreateBuffers(1, &data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vbo_);
	glCreateBuffers(1, &data->rendercmp[entity.cmp_indx_[kRenderCompPos]].ebo_);

	glNamedBufferData(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vbo_, 
		data->geometry[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.back()].vertices_.size() * sizeof(float),
		&data->geometry[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.back()].vertices_[0], GL_STATIC_DRAW);


	glNamedBufferData(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].ebo_, 
		data->geometry[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.back()].verticesIndices_.size() * sizeof(GLuint),
		&data->geometry[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.back()].verticesIndices_[0], GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, 0);
	// Back here when applying normals etc:
	glVertexArrayAttribFormat(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, 0, kRenderCompPos, GL_FLOAT, GL_FALSE, 0);
	//

	glVertexArrayVertexBuffer(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, 0, data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vbo_, 0, 3 * sizeof(GLuint));

	glVertexArrayElementBuffer(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, data->rendercmp[entity.cmp_indx_[kRenderCompPos]].ebo_);


	glCreateBuffers(1, &data->rendercmp[entity.cmp_indx_[kRenderCompPos]].tbo_);
	glCreateBuffers(1, &data->rendercmp[entity.cmp_indx_[kRenderCompPos]].etbo_);

	if (data->geometry[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.back()].texCoords_.size() > 0) {

		glNamedBufferData(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].tbo_, data->geometry[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.back()].texCoords_.size() * sizeof(float), 
			&data->geometry[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.back()].texCoords_[0], GL_STATIC_DRAW);
		glNamedBufferData(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].etbo_, data->geometry[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.back()].texCoordsIndices_.size() * sizeof(GLuint), 
			&data->geometry[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.back()].texCoordsIndices_[0], GL_STATIC_DRAW);

		glEnableVertexArrayAttrib(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, 2);
		glVertexArrayAttribBinding(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, 2, 0);
		glVertexArrayAttribFormat(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, 2, 2, GL_FLOAT, GL_FALSE, 0);

		glVertexArrayVertexBuffer(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, 2, data->rendercmp[entity.cmp_indx_[kRenderCompPos]].tbo_, 0, 2 * sizeof(GLuint));
		glVertexArrayElementBuffer(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, data->rendercmp[entity.cmp_indx_[kRenderCompPos]].etbo_);

	}

}

bool Utlop::RenderSystem::initMat(Entity& entity, RenderCtx* data, const char* path)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* text_buffer_;

	Material tmpText;
	//Debug:
	tmpText.path_ = path;

	for (unsigned int i = 0; i < data->material.size(); i++) {
		if (data->material[i].path_._Equal(tmpText.path_)) {
			data->rendercmp[entity.cmp_indx_[kRenderCompPos]].material_idx.push_back(i);
			return true;
		}
	}


	text_buffer_ = stbi_load(tmpText.path_.c_str(), &tmpText.width_, &tmpText.height_, &tmpText.bpp_, 4);

	if (text_buffer_) {


		glCreateTextures(GL_TEXTURE_2D, 1, &tmpText.diff_);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureStorage2D(tmpText.diff_, 1, GL_RGBA8, tmpText.width_, tmpText.height_);
		glTextureSubImage2D(tmpText.diff_, 0, 0, 0, tmpText.width_, tmpText.height_, GL_RGBA, GL_UNSIGNED_BYTE, text_buffer_);
		glGenerateTextureMipmap(tmpText.diff_);


		stbi_image_free(text_buffer_);

		data->material.push_back(tmpText);
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].material_idx.push_back(data->material.size() - 1);

		return true;
	}
	else {
		return false;
	}


}

void Utlop::RenderSystem::initShader(Entity& entity, RenderCtx* data)
{
	data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_ = glCreateProgram();

	loadVertexShader("../UtlopTests/src/shaders/vs.glsl", data->rendercmp[entity.cmp_indx_[kRenderCompPos]]);
	loadFragmentShader("../UtlopTests/src/shaders/fs_texture.glsl", data->rendercmp[entity.cmp_indx_[kRenderCompPos]]);

	glLinkProgram(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_);

	checkCompileErrors(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, "PROGRAM");
	checkCompileErrors(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, "LINK");
}