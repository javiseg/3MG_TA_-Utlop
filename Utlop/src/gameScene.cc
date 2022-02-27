#include "gameScene.h"
#include "GLFW/glfw3.h"
#include "core.h"
#include "imgui.h"

namespace Utlop
{
  GameScene* GameScene::current_scene_ = nullptr;
  std::vector<GameScene*> GameScene::scenes_ = std::vector<GameScene*>();

  GameScene::GameScene()
  {
    scenes_.push_back(this);
  }

  GameScene::~GameScene()
  {

  }
	GameScene* GameScene::getCurrentScene() {
		return current_scene_;
	}
	/*unsigned int GameScene::CreateGeometry()
	{
		geometryData_.push_back(std::make_shared<Utlop::Geometry>((int)geometryData_.size()));
		return geometryData_.size()-1;
	}
	std::vector<std::shared_ptr<Utlop::Geometry>> GameScene::getGeometries()
	{
		return geometryData_;
	}
	std::shared_ptr<Utlop::Geometry> GameScene::getGeometryByID(unsigned int id)
	{
		if (id < geometryData_.size()) {
			return geometryData_[id];
		}
		else {
			return nullptr;
		}
	}
	int GameScene::getGeometryID(unsigned int id)
	{
		for each (auto& geometry in geometryData_) {
			if (geometry->getID() == id) {
				return geometry->getID();
			}
		}
		return -1;
	}
	int GameScene::getGeometryByType(Geo type)
	{
		for (int i = 0; i < geometryData_.size(); i++) {
			if (geometryData_[i]->getType() == type) {
				return geometryData_[i]->getID();
			}
		}
		return -1;
	}
	int GameScene::getGeometryByType(Geo type, char* src)
	{
		for (int i = 0; i < geometryData_.size(); i++) {
			if (geometryData_[i]->getType() == type && geometryData_[i]->IsSameOBJ(src)) {
				return geometryData_[i]->getID();
			}
		}
		return -1;
	}*/
	void GameScene::addGameObject(Utlop::GameObject gO)
	{
		gameObjects_.push_back(std::make_shared<GameObject>(gO));
	}
	int GameScene::CreateMesh(Geo type)
	{
		for (int i = 0; i < meshes_.size(); i++) {
			if (meshes_[i]->type_ == type) {
				return i;
			}
		}
		Mesh newMesh;
		newMesh.type_ = type;
		newMesh.objPath_ = "";
		switch (type) {
			case kConst_Triangle: {
				newMesh.vertices_.insert(newMesh.vertices_.begin(), begin(TriangleArray), end(TriangleArray));
				newMesh.verticesIndices_.insert(newMesh.verticesIndices_.begin(), begin(TriangleArrayIndices), end(TriangleArrayIndices));
			}break;
			case kConst_Cube: {
				newMesh.vertices_.insert(newMesh.vertices_.begin(), begin(CubeArray), end(CubeArray));
				newMesh.verticesIndices_.insert(newMesh.verticesIndices_.begin(), begin(CubeArrayIndices), end(CubeArrayIndices));
			}break;
			default:
				return -1;
			}
		newMesh.setupMesh();
		meshes_.push_back(make_shared<Mesh>(newMesh));
		
		return meshes_.size() - 1;
	}
	int GameScene::CreateMesh(Geo type, char* src)
	{
		
		for (int i = 0; i < meshes_.size(); i++) {
			if (meshes_[i]->type_ == type && strcmp(meshes_[i]->objPath_, src) == 0){
				return i;
			}
			if (i == 2) {
				printf("");
			}
		}

		meshes_.push_back(make_shared<Mesh>(*loadMeshFromOBJ(src)));
		
		/**/
		printf("\nMesh Loaded\n");

		return meshes_.size() - 1;

	}
	int GameScene::getMeshIndexByType(Geo type)
	{
		for (int i = 0; i < meshes_.size(); i++) {
			if (meshes_[i]->type_ == type) {
				return i;
			}
		}
		return -1;
	}
	int GameScene::getMeshIndexByType(Geo type, const char* src)
	{
		for (int i = 0; i < meshes_.size(); i++) {
			if (meshes_[i]->type_ == type &&
				strcmp(meshes_[i]->objPath_, src) == 0) {
				return i;
			}
		}
		return -1;
	}
	std::vector<std::shared_ptr<Utlop::Mesh>> GameScene::getMeshes()
	{
		return meshes_;
	}
	void GameScene::ImGUI()
	{
		/*if (ImGui::Begin("Utlop Test")) {

			ImGui::SetWindowPos(ImVec2(960, 0), ImGuiCond_FirstUseEver);
			ImGui::SetWindowSize(ImVec2(400, 704));

			
			ImGui::End();


			ImGui::Render();
		}*/
	}
  void GameScene::init()
  {
    current_scene_ = this;
		polygon_ = false;
  }

  void GameScene::draw()
  {
    for each(auto& gameObject_ in gameObjects_)
    {
      gameObject_->draw();
    }
  }

  void GameScene::start() { }

  void GameScene::update() { }

  void GameScene::_start()
  {
    start();
		GameObject gO;
		gO.init();
		gO.setPosition(vec3(-3.0f, 0.0f, 0.0f));
		gO.setColor(glm::vec3(1.0, 0.5f, 0.0f));
		gO.addMesh(CreateMesh(kConst_OBJ, "../UtlopTests/src/obj/backpack.obj"));
		addGameObject(gO);
		
		GameObject gO2;
		gO2.init();
		gO2.setPosition(vec3(3.0f, 0.0f, 0.0f));
		gO2.setColor(glm::vec3(0.5, 1.0f, 1.0f));
		gO2.addMesh(CreateMesh(kConst_Cube));
		addGameObject(gO2);

		
		/*ImGui::CreateContext();
		ImGui::StyleColorsDark();*/

  }
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_W) {
			Core::Instance()->getCamera()->moveForward(0.3f);
		}
		if (key == GLFW_KEY_S) {
			Core::Instance()->getCamera()->moveForward(-0.3f);
		}
		if (key == GLFW_KEY_D) {
			Core::Instance()->getCamera()->moveRight(-0.3f);
		}
		if (key == GLFW_KEY_A) {
			Core::Instance()->getCamera()->moveRight(0.3f);
		}
		if (key == GLFW_KEY_Q) {
			Core::Instance()->getCamera()->moveUp(-0.3f);
		}
		if (key == GLFW_KEY_E) {
			Core::Instance()->getCamera()->moveUp(0.3f);
		}
		if (key == GLFW_KEY_J) {
			Core::Instance()->getCamera()->RotateCamera(Core::Instance()->getDeltaTime(), 100.0f, 0);
		}
		if (key == GLFW_KEY_L) {
			Core::Instance()->getCamera()->RotateCamera(Core::Instance()->getDeltaTime(), 100.0f, 1);
		}
		if (key == GLFW_KEY_I) {
			Core::Instance()->getCamera()->RotateCamera(Core::Instance()->getDeltaTime(), 100.0f, 2);
		}
		if (key == GLFW_KEY_K) {
			Core::Instance()->getCamera()->RotateCamera(Core::Instance()->getDeltaTime(), 100.0f, 3);
		}
		if (key == GLFW_KEY_C && action == GLFW_PRESS) {
			for each (auto& gameObject_ in GameScene::getCurrentScene()->gameObjects_)
			{
				gameObject_->addMesh(GameScene::getCurrentScene()->CreateMesh(kConst_Cube));

			}
		}
		if (key == GLFW_KEY_T && action == GLFW_PRESS) {
			for each (auto& gameObject_ in GameScene::getCurrentScene()->gameObjects_)
			{
				gameObject_->addMesh(GameScene::getCurrentScene()->CreateMesh(kConst_Triangle));

			}
		}
		if (key == GLFW_KEY_O && action == GLFW_PRESS) {
			for each (auto& gameObject_ in GameScene::getCurrentScene()->gameObjects_)
			{
				gameObject_->addMesh(GameScene::getCurrentScene()->CreateMesh(kConst_OBJ, "../UtlopTests/src/obj/backpack.obj"));
			}
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			if (GameScene::getCurrentScene()->polygon_) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			GameScene::getCurrentScene()->polygon_ = !GameScene::getCurrentScene()->polygon_;
		}
	}
  void GameScene::_update()
  {
    update();
		//glfwPollEvents();
		glfwSetKeyCallback(Core::Instance()->getWindow()->getWindow(), key_callback);
		
		for each (auto & gameObject_ in gameObjects_)
		{
			gameObject_->update();
		}

  }
}