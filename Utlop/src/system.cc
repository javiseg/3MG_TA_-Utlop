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
	initGeo(entity, data, "../UtlopTests/src/obj/backpack/backpack.obj");
	initMat(entity, data, "../UtlopTests/src/obj/backpack/diffuse.jpg");
	initMat(entity, data, "../UtlopTests/src/textures/default.png");
	
	setMat4fv(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_,
		data->cameracmp[0].projection_, "ProjectionMatrix");
}

void Utlop::RenderSystem::exec(Entity& entity, RenderCtx* data, DisplayList* dl)
{

	addDrawCmd(dl, data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_,
		data->material[0].diff_, data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_,
		data->geometry[data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx[0]].totalIndices_.size(),
		data->localtrcmp[entity.cmp_indx_[0]].model, data->cameracmp[0].view_);


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
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].tvao_ = data->rendercmp[0].tvao_;
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].tbo_ = data->rendercmp[0].tbo_;
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].etbo_ = data->rendercmp[0].etbo_;
	}
	else {
		Geometry geo;
		loadOBJ2(path, geo);

		const GLuint vertexPosition = 0;
		const GLuint texcoord = 1;
		const GLuint normalPosition = 2;

		data->geometry.push_back(geo);
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].geo_idx.push_back(data->geometry.size() - 1);


		glCreateVertexArrays(1, &data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_);

		glCreateBuffers(1, &data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vbo_);
		glCreateBuffers(1, &data->rendercmp[entity.cmp_indx_[kRenderCompPos]].ebo_);

		glNamedBufferData(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vbo_,
			geo.totalVertex_.size() * sizeof(float), geo.totalVertex_.data(), GL_STATIC_DRAW);
		glNamedBufferData(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].ebo_, 
			geo.totalIndices_.size() * sizeof(GLuint), geo.totalIndices_.data(), GL_STATIC_DRAW);


		glEnableVertexArrayAttrib(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, vertexPosition);
		glVertexArrayAttribBinding(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, vertexPosition, 0);
		glVertexArrayAttribFormat(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, vertexPosition, 3, GL_FLOAT, GL_FALSE, 0);


		glEnableVertexArrayAttrib(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, texcoord);
		glVertexArrayAttribBinding(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, texcoord, 0);
		glVertexArrayAttribFormat(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, texcoord, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

		glEnableVertexArrayAttrib(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, normalPosition);
		glVertexArrayAttribBinding(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, normalPosition, 0);
		glVertexArrayAttribFormat(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, normalPosition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float));



		glVertexArrayVertexBuffer(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_, 0, 
			data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vbo_, 0, 8 * sizeof(float));

		
		glVertexArrayElementBuffer(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].vao_,
			data->rendercmp[entity.cmp_indx_[kRenderCompPos]].ebo_);
	}
	
	//glUseProgram(0);
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

	stbi_set_flip_vertically_on_load(true);
	unsigned char* text_buffer_;
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
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tmpText.diff_);
		glUniform1i(glGetUniformLocation(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, "ourTexture"), 0);


		stbi_image_free(text_buffer_);

		data->material.push_back(tmpText);
		return true;
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
	glUseProgram(0);
	checkCompileErrors(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, "PROGRAM");
	checkCompileErrors(data->rendercmp[entity.cmp_indx_[kRenderCompPos]].shaderID_, "LINK");
}