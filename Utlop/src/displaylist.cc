#include "displaylist.h"
#include "mesh.h"
#include "component.h"
#include <memory>

void Utlop::DisplayList::executeOnGPU()
{
	for (auto&& list : cmdList) {
		list->executeOnGPU();
	}
  cmdList.clear();
}

Utlop::DisplayList& Utlop::addWindowClearCmd(DisplayList* dl, float r, float g, float b, float a)
{
	shared_ptr<WindowClearCmd> window_clear_cmd;
	window_clear_cmd = make_shared<WindowClearCmd>();
	window_clear_cmd->r = r;
	window_clear_cmd->g = g;
	window_clear_cmd->b = b;
	window_clear_cmd->a = a;

	dl->cmdList.push_back(move(window_clear_cmd));

	return *dl;
}

Utlop::DisplayList& Utlop::addViewPortCmd(DisplayList* dl, GLint x, GLint y, GLsizei width, GLsizei height)
{
  shared_ptr<ViewPortCmd> vp_cmd;
  vp_cmd = make_shared<ViewPortCmd>();
  vp_cmd->x = x;
  vp_cmd->y = y;
  vp_cmd->height = height;
  vp_cmd->width = width;

  dl->cmdList.push_back(move(vp_cmd));

  return *dl;
}


Utlop::DisplayList& Utlop::addDrawCmd(Utlop::DisplayList* dl, GLuint shaderId, GLuint materialID, GLuint vao, GLsizei size,
	glm::mat4 view, glm::mat4 model)
{
	shared_ptr<DrawMaterialCmd> draw_material_cmd;
	draw_material_cmd = make_shared<DrawMaterialCmd>();
	draw_material_cmd->shaderId = shaderId;
	draw_material_cmd->materialID = materialID;
	draw_material_cmd->vao = vao;
	draw_material_cmd->size = size;
	draw_material_cmd->view = view;
	draw_material_cmd->model = model;

	dl->cmdList.push_back(move(draw_material_cmd));

	return *dl;
}

Utlop::DisplayList& Utlop::addDrawMeshCmd(DisplayList* dl, Utlop::Mesh m, GLuint shader, Utlop::CameraComponent cameracmp, Utlop::LocalTRComponent localcmp, int hasLightComponent, Material mat)
{
	shared_ptr<DrawMeshCmd> drawMeshCmd;
	drawMeshCmd = make_shared<DrawMeshCmd>();

	drawMeshCmd->m = m;
	drawMeshCmd->shader = shader;
	drawMeshCmd->cameracmp = cameracmp;
  drawMeshCmd->hasLightComponent = hasLightComponent;
  drawMeshCmd->mat = mat;

	dl->cmdList.push_back(move(drawMeshCmd));

	return *dl;
}

Utlop::DisplayList& Utlop::addSetPolygonCmd(Utlop::DisplayList* dl, uint8_t on)
{
	std::shared_ptr<Utlop::SetPolygonCmd> pol;
	pol = std::make_shared<Utlop::SetPolygonCmd>();
	pol->value = on;

	dl->cmdList.push_back(move(pol));

	return *dl;
}

Utlop::DisplayList& Utlop::addSetModelViewProjection(Utlop::DisplayList* dl, GLuint shaderID, glm::mat4 projection, glm::mat4 model, glm::mat4 view)
{

	std::shared_ptr<Utlop::SetModelViewProjectionCmd> mvpCmd;
	mvpCmd = std::make_shared<Utlop::SetModelViewProjectionCmd>();
	mvpCmd->shaderID = shaderID;
	mvpCmd->m = model;
	mvpCmd->v = view;
	mvpCmd->p = projection;

	dl->cmdList.push_back(move(mvpCmd));

	return *dl;
}

Utlop::DisplayList& Utlop::addSetLightDataCmd(Utlop::DisplayList* dl, glm::vec3 color, glm::vec3 position, float intensity, 
  GLuint shaderID, vec3 camPosition, vec3 direction, int type)
{
	std::shared_ptr<Utlop::SetLightDataCmd> lightCmd;
	lightCmd = std::make_shared<Utlop::SetLightDataCmd>();

	lightCmd->color = color;
	lightCmd->position = position;
	lightCmd->intensity = intensity;
	lightCmd->shaderID = shaderID;
	lightCmd->camPosition = camPosition;
	lightCmd->direction = position;
	lightCmd->type = type;

	dl->cmdList.push_back(move(lightCmd));

	return *dl;
}

Utlop::DisplayList& Utlop::addEnableDepthTest(DisplayList* dl)
{
	std::unique_ptr<Utlop::EnableDepthCmd> depthCmd;
	depthCmd = std::make_unique<Utlop::EnableDepthCmd>();

	dl->cmdList.push_back(move(depthCmd));

	return *dl;
}

Utlop::DisplayList& Utlop::addDisableDepthTest(DisplayList* dl)
{
	std::unique_ptr<Utlop::DisableDepthCmd> depthCmd;
	depthCmd = std::make_unique<Utlop::DisableDepthCmd>();

	dl->cmdList.push_back(move(depthCmd));

	return *dl;
}

Utlop::DisplayList& Utlop::addClearDepthBufferCmd(DisplayList* dl)
{
  std::unique_ptr<Utlop::ClearDepthBufferCmd> cdbcmd;
  cdbcmd = std::make_unique<Utlop::ClearDepthBufferCmd>();

  dl->cmdList.push_back(move(cdbcmd));

  return *dl;
}

Utlop::DisplayList& Utlop::addDoFramebuffer(DisplayList* dl, GLuint shaderID, GLuint rectVAO, GLuint texture, GLuint fbo, GLuint indicessize, GLint fb_type)
{
	std::unique_ptr<Utlop::DoFrameBufferCmd> fbcmd;
	fbcmd = std::make_unique<Utlop::DoFrameBufferCmd>();

	fbcmd->shaderID = shaderID;
	fbcmd->rectVAO = rectVAO;
	fbcmd->fbTexture = texture;
	fbcmd->fbo = fbo;
  fbcmd->indicessize = indicessize;
  fbcmd->type = fb_type;

	dl->cmdList.push_back(move(fbcmd));

	return *dl;
}

Utlop::DisplayList& Utlop::addBindFramebuffer(DisplayList* dl, GLuint fboID)
{
	std::unique_ptr<Utlop::BindFramebufferCmd> fbcmd;
	fbcmd = std::make_unique<Utlop::BindFramebufferCmd>();

	fbcmd->fboID = fboID;

	dl->cmdList.push_back(move(fbcmd));

	return *dl;
}

Utlop::DisplayList& Utlop::addShadowFrameBufferCmd(DisplayList* dl, GLuint shaderID, mat4 lightProjection, GLuint activeTexture, GLuint FBtexture)
{
	std::unique_ptr<Utlop::ShadowMapCmd> sfbcmd;
	sfbcmd = std::make_unique<Utlop::ShadowMapCmd>();

  sfbcmd->activeTexture = activeTexture;
  sfbcmd->FBtexture = FBtexture;
  sfbcmd->lightProjection = lightProjection;
  sfbcmd->shaderID = shaderID;

	dl->cmdList.push_back(move(sfbcmd));

	return *dl;
}

Utlop::DisplayList& Utlop::addDrawSkybox(DisplayList* dl, GLuint shaderID, glm::vec3 position, glm::vec3 front, glm::vec3 Up, glm::mat4 projection, glm::mat4 view, GLuint vao, GLuint texture)
{
	std::unique_ptr<Utlop::SkyboxCmd> skycmd;
	skycmd = std::make_unique<Utlop::SkyboxCmd>();

	skycmd->shaderID = shaderID;
	skycmd->position = position;
	skycmd->front = front;
	skycmd->Up = Up;
	skycmd->projection = projection;
	skycmd->view = view;
	skycmd->vao = vao;
	skycmd->texture = texture;

	dl->cmdList.push_back(move(skycmd));

	return *dl;
}
