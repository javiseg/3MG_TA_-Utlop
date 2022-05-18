#include "core.h"

#include <chrono>
#include <thread>
#include <fstream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "..\include\core.h"
#include "imguiStruct.h"
#include "system.h"
#include "tiny_obj_loader.h"
#include <time.h>
#include <glm/gtc/type_ptr.hpp>
#include "tools.h"
#include "database.h"
#include "ImGuizmo.h"
#include "stb_image.h"
#define PX_SCHED_IMPLEMENTATION 1
#include "px_sched.h"

void loadCubemap(const char* path, GLuint& texture);
void loadVertexShader(const char* filename, GLuint& vertShader);
void loadFragmentShader(const char* filename, GLuint& fragShader);
void setMat4fv(GLuint shader_id, glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);
bool loadOBJ2(const char* path, Utlop::Geometry& geo);

namespace Utlop
{
	struct PerFrameData
	{
		mat4 model;
		mat4 mvp;
		vec4 cameraPos;
	};

  Core* Core::_instance = nullptr;

  Core::Core()
  {
    _fps = 0.0f;
    _frame_time_millis = 0;
		data = new RenderCtx();
		displayList = new DisplayList();
    _instance = this;
		data->framebuffer = make_unique<RenderToTexture>();
		data->shadowframebuffer = make_unique<RenderToTexture>();
		cubemap = make_unique<CubeMap>();
  }

  Core::~Core()
  {
		delete data;
		delete displayList;
		DestroyImGUI();
		glfwTerminate();
  }

	void Utlop::Core::createEntities(Core* cr) {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 1; j++) {
				int entityIdx = cr->AddEntity();
				cr->AddComponent(*cr->getData()->entities[entityIdx], kLocalTRComp);
				cr->AddComponent(*cr->getData()->entities[entityIdx], kRenderComp);
				cr->AddComponent(*cr->getData()->entities[entityIdx], kLightComp);
				//cr->AddComponent(*cr->getData()->entities[entityIdx], kTypeLightComp);
				
				cr->getData()->localtrcmp[cr->getData()->entities[entityIdx]->cmp_indx_[kLocalTRCompPos]].position -= vec3(20.0f * i, 20.0f * j, 0.0f);
			}
		}
	}
  bool Core::init(float fps)
  {
    _fps = fps;
    _frame_time_millis = 1000 / (long)_fps;
		preExecDone_ = 0;
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		polygon_ = false;
		camera_speed_ = 10.0f;
		InitSystems();
		InitComponents();

		

		//Debug:
		AddEntity();
		AddComponent(*data->entities[0], kCameraComp);
		AddComponent(*data->entities[0], kLocalTRComp);
		data->localtrcmp[data->entities[0]->cmp_indx_[kCameraCompPos]].position = vec3(0.0f, 0.0f, 55.0f);


		//Point Light
		/*
		int lightEntity = AddEntity();
		AddComponent(*data->entities[lightEntity], kLocalTRComp);
		AddComponent(*data->entities[lightEntity], kTypeLightComp);
		AddComponent(*data->entities[lightEntity], kRenderComp);
		*/
		//Directional Light
		int lightEntity = AddEntity();
		AddComponent(*data->entities[lightEntity], kLocalTRComp);
		AddComponent(*data->entities[lightEntity], kTypeLightComp);
		AddComponent(*data->entities[lightEntity], kRenderComp);
		data->typelighcmp[data->entities[lightEntity]->cmp_indx_[kTypeLightCompPos]].type = 0;
		

    /*
    int lightEntity2 = AddEntity();
    AddComponent(*data->entities[lightEntity2], kLocalTRComp);
    AddComponent(*data->entities[lightEntity2], kTypeLightComp);
    AddComponent(*data->entities[lightEntity2], kRenderComp);
    data->typelighcmp[data->entities[lightEntity2]->cmp_indx_[kTypeLightCompPos]].type = 0;
    */


		px_sched::Scheduler scheduler1;
		scheduler1.init();
		px_sched::Sync schedulerReady1;


		Core* cr = Instance();
		auto job = [cr] {
			cr->createEntities(cr);
		};
		printf("Antes\n");
		//scheduler1.run(job, &schedulerReady1);
		createEntities(cr);

		printf("Waiting for tasks to finish...\n");
		scheduler1.waitFor(schedulerReady1); // wait for all tasks to finish
		printf("Waiting for tasks to finish...DONE \n");
		printf("Despues\n");
		//AddEntity();
		
		
		
		bg_color_ = vec4(0.0f);
		bg_color_.w = 1.0f;
		bool done = glfwInit();

		getWindow()->init(1380, 780, "Utlop");

		glfwMakeContextCurrent(_window._window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			printf("Failed to initialize GLAD");
		}
		//Load Shader
		data->shaders.push_back(Shader("../UtlopTests/src/shaders/vs.glsl", "../UtlopTests/src/shaders/fs_texture.glsl"));
		
		data->shaders.push_back(Shader("../UtlopTests/src/shaders/fb_vert.glsl", "../UtlopTests/src/shaders/fb_frag.glsl"));
		data->framebuffer->initShader(data->shaders.size() - 1);
		data->framebuffer->rectangleToGPU();
		data->framebuffer->initFBO(_window.width, _window.height);
		data->framebuffer->errorCheck();
	
		data->shaders.push_back(Shader("../UtlopTests/src/shaders/shadowfb_vert.glsl", "../UtlopTests/src/shaders/shadowfb_frag.glsl"));

		data->shadowframebuffer->initShader(data->shaders.size() - 1);
		data->shadowframebuffer->rectangleToGPU();
		data->shadowframebuffer->initShadowFBO(_window.width, _window.height);
		vec3 lightPosition = vec3(0.0f, 1.0f, 1.0f);
		data->shadowframebuffer->setLightPerspective(vec3(0.0f, 1.0f, 1.0f), &data->shaders, 0);
		data->shadowframebuffer->errorCheck();

		std::string facesCubemap[6] =
		{
			"../UtlopTests/src/textures/cubemap/right.jpg",
			"../UtlopTests/src/textures/cubemap/left.jpg",
			"../UtlopTests/src/textures/cubemap/top.jpg",
			"../UtlopTests/src/textures/cubemap/bottom.jpg",
			"../UtlopTests/src/textures/cubemap/front.jpg",
			"../UtlopTests/src/textures/cubemap/back.jpg"
		};

		cubemap->createBuffers();
		cubemap->loadTextures(facesCubemap);
		data->shaders.push_back(Shader("../UtlopTests/src/shaders/skybox_vert.glsl", "../UtlopTests/src/shaders/skybox_frag.glsl"));
		cubemap->shader_idx = data->shaders.size() - 1;
		glUniform1i(glGetUniformLocation(data->shaders[cubemap->shader_idx].id, "skybox"), 0);


		vector<string> robotTextures;
		robotTextures.push_back("../UtlopTests/src/obj/robot/diffuse.jpg");
		InitMesh("../UtlopTests/src/obj/robot/robot.obj", robotTextures);
		data->obj_str_type.push_back("Robot");

		vector<string> cubeTextures;
		cubeTextures.push_back("../UtlopTests/src/textures/white.png");
		InitMesh("../UtlopTests/src/obj/lightcube.obj", cubeTextures);
		data->obj_str_type.push_back("White cube");

		vector<string> helmetTextures;
		helmetTextures.push_back("../UtlopTests/src/obj/helmet/diffuse.png");
		helmetTextures.push_back("../UtlopTests/src/obj/helmet/specular.png");
		//helmetTextures.push_back("../UtlopTests/src/obj/helmet/normal.png");
		InitMesh("../UtlopTests/src/obj/helmet/helmet.obj", helmetTextures);
		data->obj_str_type.push_back("Helmet");

		vector<string> jupitertextures;
		jupitertextures.push_back("../UtlopTests/src/obj/sphere/jupiter.jpg");
		//jupitertextures.push_back("../UtlopTests/src/obj/helmet/specular.png");
		InitMesh("../UtlopTests/src/obj/sphere/planet.obj", jupitertextures);
		data->obj_str_type.push_back("Planet");
		
		vector<string> floorTextures;
		floorTextures.push_back("../UtlopTests/src/obj/floor/diffuse.png");
		InitMesh("../UtlopTests/src/obj/floor/floor.obj", floorTextures);
		data->obj_str_type.push_back("Floor");

		vector<string> newCubeTextures;
		newCubeTextures.push_back("../UtlopTests/src/obj/cube/diffuse1.jpg");
		newCubeTextures.push_back("../UtlopTests/src/obj/cube/specular.png");
		//newCubeTextures.push_back("../UtlopTests/src/obj/cube/normal.jpg");
		InitMesh("../UtlopTests/src/obj/cube/cube.obj", newCubeTextures);
		data->obj_str_type.push_back("Wood cube");

    return done;
  }


  void Core::start()
  {
    if (_window.isValid())
    {
			//// Thread

			px_sched::Scheduler scheduler;
			scheduler.init();
			px_sched::Sync schedulerReady;
			
			Core* cr = Instance();
			auto preSched = [cr] {
				cr->PreExecSystems();
			};
			auto sched = [cr] {
				cr->ExecSystems();
			};


			GLint version_max, version_min;
			glGetIntegerv(GL_MAJOR_VERSION, &version_max);
			glGetIntegerv(GL_MINOR_VERSION, &version_min);
			printf("Version: %d.%d \n", version_max, version_min);

			float lastFrame = (float)glfwGetTime();
			InitImGUI();
			
			
			//scheduler.run(preSched, &schedulerReady);
			PreExecSystems();
			//scheduler.run(preSched, &schedulerReady);
			//scheduler.waitFor(schedulerReady);

      while (!glfwWindowShouldClose(_window._window))
      {
        std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();

        if (glfwGetKey(_window._window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
          glfwSetWindowShouldClose(_window._window, GL_TRUE);
			
				deltaTime_ = (float)glfwGetTime() - lastFrame;
				lastFrame = (float)glfwGetTime();
				
				
				MoveCamera();


				ChangeShader(data->shadowframebuffer->shader_idx);
				data->shaders[data->shadowframebuffer->shader_idx].Activate();
				addEnableDepthTest(displayList);
				addViewPortCmd(displayList, 0, 0, data->shadowframebuffer->width, data->shadowframebuffer->height);
        addBindFramebuffer(displayList, data->shadowframebuffer->FBOid);
        addClearDepthBufferCmd(displayList);
				ExecSystems();
				displayList->executeOnGPU();

        addBindFramebuffer(displayList, 0);
				addViewPortCmd(displayList, 0, 0, _window.width, _window.height);
				addBindFramebuffer(displayList, data->framebuffer->FBOid);
				addWindowClearCmd(displayList, bg_color_.x, bg_color_.y, bg_color_.z, bg_color_.w);
        Update();
				addDoFramebuffer(displayList, data->shaders[data->framebuffer->shader_idx].id, data->framebuffer->rectVAO, data->framebuffer->FBtexture, data->framebuffer->FBOid);
				displayList->executeOnGPU();

				ImGUI();

        glfwSwapBuffers(_window._window);
        glfwPollEvents();

        std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
        std::this_thread::sleep_until(start_time + std::chrono::milliseconds(_frame_time_millis));
				
				//long current_time = (long)(end_time.time_since_epoch().count() - start_time.time_since_epoch().count());
				//printf("%f\n", deltaTime_);
#ifdef DEBUG
        printf("Desired FPS: %f - Current FPS: %f\n", _fps, current_time / 1000.0f);
#endif // DEBUG
      }
    }
  }

	void Core::Update()
	{
		
    addEnableDepthTest(displayList);

    ChangeShader(0);

    addShadowFrameBufferCmd(displayList, data->shaders[0].id, data->shadowframebuffer->lightProjection, 2, data->shadowframebuffer->FBtexture);

    addDrawSkybox(displayList, data->shaders[cubemap->shader_idx].id, data->localtrcmp[0].position,
      data->cameracmp[0].front_, data->cameracmp[0].Up,
      data->cameracmp[0].projection_, data->cameracmp[0].view_,
      cubemap->vao, cubemap->texture);
    ExecSystems();

    addDisableDepthTest(displayList);

	}

  Utlop::Window* Core::getWindow()
  {
    return &_window;
  }

	float Core::getDeltaTime()
	{
		return deltaTime_;
	}

	int Core::AddEntity()
	{
		Entity newEntity;

		newEntity.componentsID_ = 0;
		for (int i = 0; i < kMaxComponents; i++) {
			newEntity.cmp_indx_[i] = -1;
		}		
		newEntity.entityIdx = data->entities.size();
		data->entities.push_back(make_shared<Entity>(newEntity));

		return newEntity.entityIdx;
	}

	void Core::AddComponent(Entity& entity, Utlop::ComponentID id)
	{
		if ((entity.componentsID_ & id) == 0) {

			entity.AddComponent(id);

			switch (id) {
				case kLocalTRComp: {
						data->localtrcmp.push_back(LocalTRComponent());
						entity.cmp_indx_[kLocalTRCompPos] = (int)(data->localtrcmp.size() - 1);
					break;
				}
				case kWorldTRComp: {
						data->worldtrcmp.push_back(WorldTRComponent());
						entity.cmp_indx_[kWorldTRCompPos] = (int)(data->worldtrcmp.size() - 1);
					break;
				}
				case kCameraComp: {
						data->cameracmp.push_back(CameraComponent());
						entity.cmp_indx_[kCameraCompPos] = (int)(data->cameracmp.size() - 1);
					break;
				}
				case kRenderComp: {
						data->rendercmp.push_back(RenderComponent());
						entity.cmp_indx_[kRenderCompPos] = (int)(data->rendercmp.size() - 1);
					break;
				}

				case kHeritageComp: {
						printf("\nTO DO: Add heritage comp at Add component \n");
					break;
				}

				case kLightComp: {
						data->lightcmp.push_back(LightComponent());
						entity.cmp_indx_[kLightCompPos] = (int)(data->lightcmp.size() - 1);
					break;
				}

				case kTypeLightComp: {
					data->typelighcmp.push_back(TypeLightComponent());
					entity.cmp_indx_[kTypeLightCompPos] = (int)(data->typelighcmp.size() - 1);
					break;
				}
			}

		}
		else {
			printf("\nComponent already in Entity\n");
		}
	
	}

  void Core::AddAllComponents(Entity& entity, GLuint components)
  {
    if ((components & kLocalTRComp) == kLocalTRComp) {
      data->localtrcmp.push_back(LocalTRComponent());
      entity.cmp_indx_[kLocalTRCompPos] = (int)(data->localtrcmp.size() - 1);
    }
    if ((components & kWorldTRComp) == kWorldTRComp) {
      data->worldtrcmp.push_back(WorldTRComponent());
      entity.cmp_indx_[kWorldTRCompPos] = (int)(data->worldtrcmp.size() - 1);
    }
    if ((components & kCameraComp) == kCameraComp) {
      data->cameracmp.push_back(CameraComponent());
      entity.cmp_indx_[kCameraCompPos] = (int)(data->cameracmp.size() - 1);
    }
    if ((components & kRenderComp) == kRenderComp) {
      data->rendercmp.push_back(RenderComponent());
      entity.cmp_indx_[kRenderCompPos] = (int)(data->rendercmp.size() - 1);
    }
    if ((components & kLightComp) == kLightComp) {
      data->lightcmp.push_back(LightComponent());
      entity.cmp_indx_[kLightCompPos] = (int)(data->lightcmp.size() - 1);
    }
    if ((components & kTypeLightComp) == kTypeLightComp) {
      data->typelighcmp.push_back(TypeLightComponent());
      entity.cmp_indx_[kTypeLightCompPos] = (int)(data->typelighcmp.size() - 1);
    }
  }


	vector<Texture> Core::InitMaterials(RenderCtx* data, vector<string> texturePaths)
	{
		vector<Texture> textures;
		for (int i = 0; i < texturePaths.size(); i++) {
			if(i == 0)
				textures.push_back(Texture(texturePaths[i].c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE));
			else if(i == 1)
				textures.push_back(Texture(texturePaths[i].c_str(), "specular", 1, GL_RED, GL_UNSIGNED_BYTE));
			else if(i  == 2)
				textures.push_back(Texture(texturePaths[i].c_str(), "normal", 1, GL_RGB, GL_UNSIGNED_BYTE));
		}
		return textures;
	}

	Geometry Core::InitGeometry(RenderCtx* data, const char* path)
	{
		Geometry geo;
		loadOBJ2(path, geo);

		return geo;
	}

	void Core::ChangeMesh(Entity& entity, RenderCtx* data, int option)
	{
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].mesh_idx[0] = option;
	}

  void Core::ClearDataLists(vector<std::vector<int>>* selectedType)
  {
    data->entities.erase(data->entities.begin() + 1, data->entities.end());
    data->localtrcmp.erase(data->localtrcmp.begin() + 1, data->localtrcmp.end());
    data->rendercmp.clear();
    data->lightcmp.clear();
    data->typelighcmp.clear();
    data->worldtrcmp.clear();
    selectedType->clear();
  }

  void Core::InitMesh(string geometryPath, vector<string> texturePath)
	{
		Geometry geo = InitGeometry(data, geometryPath.c_str());
		
		Mesh newMesh(geo.totalVertex_, geo.totalIndices_, InitMaterials(data, texturePath), geometryPath);
		data->meshes.push_back(newMesh);
   
	}


	void Core::InitComponents()
	{
		data->kComponentMap.insert(make_pair(kLocalTRComp, LocalTRComponent()));
		data->kComponentMap.insert(make_pair(kWorldTRComp, WorldTRComponent()));
		data->kComponentMap.insert(make_pair(kRenderComp, RenderComponent()));
		data->kComponentMap.insert(make_pair(kCameraComp, CameraComponent()));
		data->kComponentMap.insert(make_pair(kHeritageComp, HeritageComponent()));
		data->kComponentMap.insert(make_pair(kLightComp, LightComponent()));
		data->kComponentMap.insert(make_pair(kTypeLightComp, TypeLightComponent()));
	}

	void Core::InitSystems()
	{
		data->sys.push_back(make_shared<LocalTRSystem>());
		data->sys.push_back(make_shared<WorldTRSystem>());
		data->sys.push_back(make_shared<CameraSystem>());
		data->sys.push_back(make_shared<RenderSystem>());
		data->sys.push_back(make_shared<HeritageSystem>());
		data->sys.push_back(make_shared<LightSystem>());
		data->sys.push_back(make_shared<TypeLightSystem>());
	}

	void Core::PreExecSystems()
	{
		for (unsigned int i = 0; i < data->sys.size(); i++) {
			for (unsigned int h = 0; h < data->entities.size(); h++) {
				int out = data->entities[h]->componentsID_ & data->sys[i]->id_;
				if (out == data->sys[i]->id_)
					data->sys[i]->preExec(*data->entities[h], data);
			}
		}
		preExecDone_ = 1;
	}

	void Core::PreExecSystem(Entity& entity)
	{
		for (unsigned int i = 0; i < data->sys.size(); i++) {	
			int out = entity.componentsID_ & data->sys[i]->id_;
			if (out == data->sys[i]->id_)
				data->sys[i]->preExec(entity, data);
		}
	}

	void Core::ExecSystems()
	{
		for (unsigned int i = 0; i < data->sys.size(); i++) {
			for (unsigned int h = 0; h < data->entities.size(); h++) {
				int out = data->entities[h]->componentsID_ & data->sys[i]->id_;
				if (out == data->sys[i]->id_)
					data->sys[i]->exec(*data->entities[h], data, displayList);
			}
		}
	}

	void Core::ExecSystems2()
	{
		for (unsigned int h = 0; h < data->entities.size(); h++) {
			int out = data->entities[h]->componentsID_ & data->sys[kRenderCompPos]->id_;
			if (out == data->sys[kRenderCompPos]->id_)
				data->sys[kRenderCompPos]->exec(*data->entities[h], data, displayList);
		}
	}

	void Core::ChangeShader(GLuint shader_idx)
	{
		for (unsigned int h = 0; h < data->rendercmp.size(); h++) {
			data->rendercmp[h].shader_idx = shader_idx;
		}
	}


  Core* Core::Instance()
  {
    return _instance;
  }
	RenderCtx* Core::getData()
	{
		return data;
	}
	DisplayList* Core::getDisplayList()
	{
		return displayList;
	}
	float Core::getCameraSpeed()
	{
		return camera_speed_;
	}

  void EditTransform(const Utlop::CameraComponent& camera, LocalTRComponent& transform, int n)
  {
    ImGuizmo::BeginFrame();

    std::string slidername;
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

    
    vec3 position;
    vec3 rotation;
    vec3 scale;

    if (ImGui::IsKeyPressed(90))
      mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(69))
      mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(82)) // r Key
      mCurrentGizmoOperation = ImGuizmo::SCALE;
    slidername = "Translate " + std::to_string(n);
    if (ImGui::RadioButton(slidername.c_str(), mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
      mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    slidername = "Rotate " + std::to_string(n);
    if (ImGui::RadioButton(slidername.c_str(), mCurrentGizmoOperation == ImGuizmo::ROTATE))
      mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    slidername = "Scale " + std::to_string(n);
    if (ImGui::RadioButton(slidername.c_str(), mCurrentGizmoOperation == ImGuizmo::SCALE))
      mCurrentGizmoOperation = ImGuizmo::SCALE;
    

    glm::mat4 modelMat = transform.model;


    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGuizmo::Manipulate(value_ptr(camera.view_), value_ptr(camera.projection_),
      mCurrentGizmoOperation, mCurrentGizmoMode, value_ptr(modelMat),
      NULL, nullptr);

    transform.model = modelMat;


    ImGuizmo::DecomposeMatrixToComponents(value_ptr(transform.model),
      value_ptr(transform.position),
      value_ptr(transform.rotation),
      value_ptr(transform.scale));

  }

	void Core::ImGUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(300, 780));
    static vector<std::vector<int>> selectedType;
		if (ImGui::Begin("Utlop Engine")) {

			ImGui::ColorEdit4("Color", &bg_color_[0]);


			if (ImGui::Button("Add Agent")) {
				int entityIdx = AddEntity();
				AddComponent(*data->entities[entityIdx], kLocalTRComp);
				AddComponent(*data->entities[entityIdx], kRenderComp);
				AddComponent(*data->entities[entityIdx], kLightComp);
				PreExecSystem(*data->entities[entityIdx]);
			} ImGui::SameLine();

      if (ImGui::Button("Load DataBase")) {
        DataBase db;
        ClearDataLists(&selectedType);
        db.LoadDatabase("../UtlopTests/src/db/scene.db", data);
        PreExecSystems();
      }

			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Current GameObjects");
			ImGui::BeginChild("GameObject");

			vec3 position;
			vec3 rotation;
			vec3 scale;
			
			const char* obj_type[]{ "Robot", "White Cube", "Helmet", "Container", "Car", "Cube"};
			



			for (int n = 0; n < data->entities.size(); n++) {
				if (selectedType.size()<= n)
					selectedType.push_back(vector<int>(0));
				if (data->entities[n]->cmp_indx_[kLocalTRCompPos] != -1) {
					ImGui::Text("%04d: Object", n);
					position = data->localtrcmp[data->entities[n]->cmp_indx_[kLocalTRCompPos]].position;
					rotation = data->localtrcmp[data->entities[n]->cmp_indx_[kLocalTRCompPos]].rotation;
					scale = data->localtrcmp[data->entities[n]->cmp_indx_[kLocalTRCompPos]].scale;
					ImGui::SetCursorPosX(100.0f);
          mat4 model = data->localtrcmp[data->entities[n]->cmp_indx_[kLocalTRCompPos]].model;
          float matrixTranslation[3], matrixRotation[3], matrixScale[3];
          
          /*std::string slidername = "Location of " + std::to_string(n);
					ImGui::Text("Location");
					ImGui::SliderFloat3(slidername.c_str(), &position[0], -100.0f, 100.0f);
					data->localtrcmp[data->entities[n]->cmp_indx_[kLocalTRCompPos]].position = position;
					ImGui::SetCursorPosX(100.0f);
					std::string slidername2 = "Rotation of " + std::to_string(n);
					ImGui::Text("Rotation");
					ImGui::SliderFloat3(slidername2.c_str(), &rotation[0], -359.0f, 359.0f);
					data->localtrcmp[data->entities[n]->cmp_indx_[kLocalTRCompPos]].rotation = rotation;
					ImGui::SetCursorPosX(100.0f);
					std::string slidername3 = "Scale of " + std::to_string(n);
					ImGui::Text("Scale");
					ImGui::SliderFloat3(slidername3.c_str(), &scale[0], 0.0f, 10.0f);
					data->localtrcmp[data->entities[n]->cmp_indx_[kLocalTRCompPos]].scale = scale;
				*/}
				if (data->entities[n]->cmp_indx_[kRenderCompPos] != -1 && data->entities[n]->cmp_indx_[kTypeLightCompPos] == -1) {
          
          EditTransform(data->cameracmp[0], data->localtrcmp[data->entities[n]->cmp_indx_[kLocalTRCompPos]], n);
        
         

          char s[10];
					itoa(n, s, 10);
					selectedType[n].push_back(data->rendercmp[data->entities[n]->cmp_indx_[kRenderCompPos]].mesh_idx[0]);
					ImGui::ListBox(s, &selectedType[n][0], &data->obj_str_type[0], data->obj_str_type.size());
					ChangeMesh(*data->entities[n], data, selectedType[n][0]);
				}
				else if(data->entities[n]->cmp_indx_[kTypeLightCompPos] != -1){
					static int e = 0;
          std::string radioname = "PointLight " + std::to_string(n);
					ImGui::RadioButton(radioname.c_str(), &data->typelighcmp[data->entities[n]->cmp_indx_[kTypeLightCompPos]].type, 0); ImGui::SameLine();
          radioname = "DirectLight " + std::to_string(n);
          ImGui::RadioButton(radioname.c_str(), &data->typelighcmp[data->entities[n]->cmp_indx_[kTypeLightCompPos]].type, 1); ImGui::SameLine();
          radioname = "SpotLight " + std::to_string(n);
          ImGui::RadioButton(radioname.c_str(), &data->typelighcmp[data->entities[n]->cmp_indx_[kTypeLightCompPos]].type, 2);
					//data->typelighcmp[data->entities[n]->cmp_indx_[kTypeLightCompPos]].type = (float)e;
				}

				
			}

			ImGui::EndChild();

			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		
	}
	void Core::InitImGUI()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(_window.getWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}
	void Core::DestroyImGUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}


	void Core::MoveCamera()
	{
		if (glfwGetKey(_window.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
			Utlop::Core::Instance()->getData()->localtrcmp[0].position +=
				Utlop::Core::Instance()->getData()->cameracmp[0].front_
				* Utlop::Core::Instance()->getCameraSpeed() * Utlop::Core::Instance()->getDeltaTime();
		}

		if (glfwGetKey(_window.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
			Utlop::Core::Instance()->getData()->localtrcmp[0].position -=
				Utlop::Core::Instance()->getData()->cameracmp[0].front_
				* Utlop::Core::Instance()->getCameraSpeed() * Utlop::Core::Instance()->getDeltaTime();
		}

		if (glfwGetKey(_window.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
			Utlop::Core::Instance()->getData()->localtrcmp[0].position +=
				Utlop::Core::Instance()->getData()->cameracmp[0].Right *
				Utlop::Core::Instance()->getCameraSpeed() * Utlop::Core::Instance()->getDeltaTime();
		}

		if (glfwGetKey(_window.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
			Utlop::Core::Instance()->getData()->localtrcmp[0].position -=
				Utlop::Core::Instance()->getData()->cameracmp[0].Right *
				Utlop::Core::Instance()->getCameraSpeed() * Utlop::Core::Instance()->getDeltaTime();
		}
		if (glfwGetKey(_window.getWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
			Utlop::Core::Instance()->getData()->localtrcmp[0].position +=
				Utlop::Core::Instance()->getData()->cameracmp[0].Up *
				Utlop::Core::Instance()->getCameraSpeed() * Utlop::Core::Instance()->getDeltaTime();
		}
		if (glfwGetKey(_window.getWindow(), GLFW_KEY_E) == GLFW_PRESS) {
			Utlop::Core::Instance()->getData()->localtrcmp[0].position -=
				Utlop::Core::Instance()->getData()->cameracmp[0].Up *
				Utlop::Core::Instance()->getCameraSpeed() * Utlop::Core::Instance()->getDeltaTime();
		}

		if (glfwGetKey(_window.getWindow(), GLFW_KEY_J) == GLFW_PRESS) {
			Utlop::Core::Instance()->getData()->cameracmp[0].yaw_ -= Utlop::Core::Instance()->getCameraSpeed() * 10.0f * Utlop::Core::Instance()->getDeltaTime();
		}
		if (glfwGetKey(_window.getWindow(), GLFW_KEY_L) == GLFW_PRESS) {
			Utlop::Core::Instance()->getData()->cameracmp[0].yaw_ += Utlop::Core::Instance()->getCameraSpeed() * 10.0f * Utlop::Core::Instance()->getDeltaTime();
		}
		if (glfwGetKey(_window.getWindow(), GLFW_KEY_I) == GLFW_PRESS) {
			if(Utlop::Core::Instance()->getData()->cameracmp[0].pitch_ < 85.0f)
				Utlop::Core::Instance()->getData()->cameracmp[0].pitch_ += Utlop::Core::Instance()->getCameraSpeed() * 10.0f * Utlop::Core::Instance()->getDeltaTime();
		}
		if (glfwGetKey(_window.getWindow(), GLFW_KEY_K) == GLFW_PRESS) {
			if (Utlop::Core::Instance()->getData()->cameracmp[0].pitch_ > -85.0f)
				Utlop::Core::Instance()->getData()->cameracmp[0].pitch_ -= Utlop::Core::Instance()->getCameraSpeed() * 10.0f * Utlop::Core::Instance()->getDeltaTime();
		}


		if (glfwGetKey(_window.getWindow(), GLFW_KEY_P) == GLFW_PRESS) {
			if (Utlop::Core::Instance()->polygon_) {
				addSetPolygonCmd(displayList, 0);
			}
			else {
				addSetPolygonCmd(displayList, 1);
			}
			Utlop::Core::Instance()->polygon_ = !Utlop::Core::Instance()->polygon_;
		}
	}

}
