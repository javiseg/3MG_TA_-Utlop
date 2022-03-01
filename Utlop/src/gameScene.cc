#include "gameScene.h"
#include "GLFW/glfw3.h"
#include "core.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

	void GameScene::addGameObject(Utlop::GameObject gO)
	{
		gameObjects_.push_back(std::make_shared<GameObject>(gO));
	}
	void GameScene::addDefaultGameObject()
	{
		GameObject gO;
		gO.init();
		gO.setPosition(vec3(0.0f, 0.0f, 0.0f));
		gO.setColor(glm::vec3(0.5, 1.0f, 0.0f));
		gO.addMesh(CreateMesh(kConst_Cube));
		addGameObject(gO);
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
	std::vector<std::shared_ptr<Utlop::Mesh>>& GameScene::getMeshes()
	{
		return meshes_;
	}
	void GameScene::AddTexture(string path)
	{
		stbi_set_flip_vertically_on_load(true);
		unsigned char* text_buffer_;

		Texture tmpText;

		text_buffer_ = stbi_load(path.c_str(), &tmpText.width_, &tmpText.height_, &tmpText.bpp_, 4);

		if (text_buffer_) {


			glCreateTextures(GL_TEXTURE_2D, 1, &tmpText.id_);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureStorage2D(tmpText.id_, 1, GL_RGBA8, tmpText.width_, tmpText.height_);
			glTextureSubImage2D(tmpText.id_, 0, 0, 0, tmpText.width_, tmpText.height_, GL_RGBA, GL_UNSIGNED_BYTE, text_buffer_);
			glGenerateTextureMipmap(tmpText.id_);
			

			stbi_image_free(text_buffer_);

			textureData_.push_back(make_shared<Texture>(tmpText));
		}
		
	}
	void GameScene::ImGUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowPos(ImVec2(0, 0)); 
		ImGui::SetNextWindowSize(ImVec2(300, 780));
		if (ImGui::Begin("Utlop Engine")) {
			
			ImGui::ColorEdit4("Color", &bColor[0]);
			glClearColor(bColor.x, bColor.y, bColor.z, 1.0f);
			
			ImGui::Text("Objects");
			
			if (ImGui::Button("Add Game Object")) {
				addDefaultGameObject();
			}


			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Current GameObjects");
			ImGui::BeginChild("GameObject");
			vec3 position, object_position;
			for (int n = 0; n < gameObjects_.size(); n++) {
				ImGui::Text("%02d: Object", n);
				object_position = gameObjects_[n]->getPosition();
				std::string slidername ="Object " + std::to_string(n);
				ImGui::SliderFloat3(slidername.c_str(), &object_position[0], -10.0f, 10.0f);
				gameObjects_[n]->setPosition(object_position);
				for (int i = 0; i < gameObjects_[n]->getMeshIndices().size(); i++) {
					position = gameObjects_[n]->getMeshPosition(i);
					ImGui::SetCursorPosX(100.0f);
					ImGui::Text("%02d: Mesh", gameObjects_[n]->getMeshIndices()[i]);
					std::string slidername = "Mesh " + std::to_string(i) + " Object " + std::to_string(n);
					ImGui::SliderFloat3(slidername.c_str(), &position[0], -10.0f, 10.0f);
					gameObjects_[n]->setMeshPosition(i, position);
				}
			}
				
			ImGui::EndChild();
			
			
			
			
			
			
			
			
			
			
			
			
			ImGui::End();




			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		
		/*if (ImGui::Begin("Utlop Test")) {

			
		}*/
	}
  void GameScene::InitImGUI()
  {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(Core::Instance()->getWindow()->getWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 460");
  }
	void GameScene::DestroyImGUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
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

	void GameScene::destroy()
	{
		for each (auto & texture in textureData_) {
			glDeleteTextures(1, &texture->id_);
		}
		DestroyImGUI();
	}

  void GameScene::_start()
  {
    start();

		InitImGUI();
		bColor = vec4(0.0f);

		AddTexture("../UtlopTests/src/textures/default.png");
		AddTexture("../UtlopTests/src/textures/texture.jpg");


		GameObject gO;
		gO.init();
		gO.setPosition(vec3(0.0f, 0.0f, 0.0f));
		gO.setColor(glm::vec3(1.0, 0.5f, 0.0f));
		gO.addMesh(CreateMesh(kConst_OBJ, "../UtlopTests/src/obj/backpack.obj"));
		addGameObject(gO);
		
		/*GameObject gO2;
		gO2.init();
		gO2.setPosition(vec3(3.0f, 0.0f, 0.0f));
		gO2.setColor(glm::vec3(0.5, 1.0f, 1.0f));*/
		//gO2.addMesh(CreateMesh(kConst_Cube));
		//addGameObject(gO2);

		
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