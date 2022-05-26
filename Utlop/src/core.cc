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

bool loadOBJ2(const char* path, Utlop::Geometry& geo);

namespace Utlop
{
  const unsigned int kWidth = 1680;
  const unsigned int kHeight = 720;

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
		data->silhoutteframebuffer = make_unique<RenderToTexture>();
		cubemap = make_unique<CubeMap>();
  }

  Core::~Core()
  {
		delete data;
		delete displayList;
		DestroyImGUI();
		glfwTerminate();
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

    //Camera
		AddEntity();
		AddComponent(*data->entities[0], kCameraComp);
		AddComponent(*data->entities[0], kLocalTRComp);
		data->localtrcmp[data->entities[0]->cmp_indx_[kCameraCompPos]].position = vec3(0.0f, 0.0f, 55.0f);

		//Light
		int lightEntity = AddEntity();
		AddComponent(*data->entities[lightEntity], kLocalTRComp);
		AddComponent(*data->entities[lightEntity], kTypeLightComp);
		AddComponent(*data->entities[lightEntity], kRenderComp);
		data->typelighcmp[data->entities[lightEntity]->cmp_indx_[kTypeLightCompPos]].type = 0;
	

		px_sched::Scheduler scheduler1;
		scheduler1.init();
		px_sched::Sync schedulerReady1;


		Core* cr = Instance();

		scheduler1.waitFor(schedulerReady1);

		bg_color_ = vec4(0.0f);
		bg_color_.w = 1.0f;
		bool done = glfwInit();

		getWindow()->init(kWidth, kHeight, "Utlop");

		glfwMakeContextCurrent(_window._window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			printf("Failed to initialize GLAD");
		}
		//Load Default Shader
		data->shaders.push_back(Shader("../UtlopTests/src/shaders/vs.glsl", "../UtlopTests/src/shaders/fs_texture.glsl"));
		
    // Load Frame buffer shader
		data->shaders.push_back(Shader("../UtlopTests/src/shaders/fb_vert.glsl", "../UtlopTests/src/shaders/fb_frag.glsl"));
		data->framebuffer->initShader(data->shaders.size() - 1);
		data->framebuffer->rectangleToGPU();
		data->framebuffer->initFBO(kWidth - 300.0f, kHeight);
		data->framebuffer->errorCheck();
	
    // Load Shadow framebuffer shader
		data->shaders.push_back(Shader("../UtlopTests/src/shaders/shadowfb_vert.glsl", "../UtlopTests/src/shaders/shadowfb_frag.glsl"));

		data->shadowframebuffer->initShader(data->shaders.size() - 1);
		data->shadowframebuffer->rectangleToGPU();
		data->shadowframebuffer->initShadowFBO(kWidth - 300.0f, kHeight);
		vec3 lightPosition = vec3(0.0f, 1.0f, 1.0f);
    TypeLightComponent tmplight;
    tmplight.position = vec3(1.0f);
    tmplight.type = 0;
		data->shadowframebuffer->setLightPerspective(tmplight, &data->shaders);
		data->shadowframebuffer->errorCheck();


    ////////////////Test///////////////////////////
    // Load Silhoutte buffer shader
    data->shaders.push_back(Shader("../UtlopTests/src/shaders/silhoutte_vert.glsl", "../UtlopTests/src/shaders/silhoutte_frag.glsl"));
    data->silhoutteframebuffer->initShader(data->shaders.size() - 1);
    data->silhoutteframebuffer->rectangleToGPU();
    data->silhoutteframebuffer->initFBO(kWidth - 300.0f, kHeight);
    data->silhoutteframebuffer->errorCheck();
    //////////////////////////////////////////






		std::string facesCubemap[6] =
		{
			"../UtlopTests/src/textures/cubemap/right.jpg",
			"../UtlopTests/src/textures/cubemap/left.jpg",
			"../UtlopTests/src/textures/cubemap/top.jpg",
			"../UtlopTests/src/textures/cubemap/bottom.jpg",
			"../UtlopTests/src/textures/cubemap/front.jpg",
			"../UtlopTests/src/textures/cubemap/back.jpg"
		};
    // Inits cubemap
    data->shaders.push_back(Shader("../UtlopTests/src/shaders/skybox_vert.glsl", "../UtlopTests/src/shaders/skybox_frag.glsl"));
    cubemap->shader_idx = data->shaders.size() - 1;

		cubemap->createBuffers();
		cubemap->loadTextures(facesCubemap, data);
		

    // Init White material
    Material defaultWhite;
    vector<string> defaultStrings;
    defaultStrings.push_back("../UtlopTests/src/textures/white.png");
    defaultWhite = InitMaterial(defaultStrings);
    data->mat_str_type.push_back("White Material");
    data->materials.push_back(defaultWhite);

    // Init Cube OBJ
		InitMesh("../UtlopTests/src/obj/cube/cube.obj");
		data->obj_str_type.push_back("Cube");

    // Init helmet material
    Material helmet;
    vector<string> helmetStrings;
    helmetStrings.push_back("../UtlopTests/src/textures/helmet/diffuse.png");
    helmetStrings.push_back("../UtlopTests/src/textures/helmet/specular.png");
    helmet = InitMaterial(helmetStrings);
    data->mat_str_type.push_back("Helmet Material");
    data->materials.push_back(helmet);

    // Init Helmet OBJ
    InitMesh("../UtlopTests/src/obj/helmet/helmet.obj");
    data->obj_str_type.push_back("Helmet");


    // Init Floor material
    Material floor;
    vector<string> floorStrings;
    floorStrings.push_back("../UtlopTests/src/textures/wood/diffuse.png");
    floorStrings.push_back("../UtlopTests/src/textures/wood/specular.png");
    floor = InitMaterial(floorStrings);
    data->mat_str_type.push_back("Wood Material");
    data->materials.push_back(floor);
    
    // Init Floor OBJ
    InitMesh("../UtlopTests/src/obj/floor/floor.obj");
    data->obj_str_type.push_back("Floor");

    // Init Material Brick
    Material brick;
    vector<string> brickStrings;
    brickStrings.push_back("../UtlopTests/src/textures/bricks/diffuse.png");
    brickStrings.push_back("../UtlopTests/src/textures/bricks/specular.png");
    brick = InitMaterial(brickStrings);
    data->mat_str_type.push_back("Bricks Material");
    data->materials.push_back(brick);

    // Init Capsule OBJ
    InitMesh("../UtlopTests/src/obj/capsule/capsule.obj");
    data->obj_str_type.push_back("Capsule");


    int silhoutteEntity = AddEntity();
    AddComponent(*data->entities[silhoutteEntity], kLocalTRComp);
    AddComponent(*data->entities[silhoutteEntity], kLightComp);
    AddComponent(*data->entities[silhoutteEntity], kRenderComp);
    AddComponent(*data->entities[silhoutteEntity], kSilhoutteComp);



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
			px_sched::Sync schedulerUpdateReady;
			px_sched::Sync schedulerShadowMapUpdateReady;
      px_sched::Job g;

			Core* cr = Instance();
			auto preSched = [cr] {
				cr->PreExecSystems();
			};
      scheduler.run(preSched, &schedulerReady);

			auto schedUpdate = [cr] {
				addBindFramebuffer(cr->displayList, 0);
        addViewPortCmd(cr->displayList, 0, 0, kWidth - 300.0f, kHeight);
        addBindFramebuffer(cr->displayList, cr->data->framebuffer->FBOid);
        addWindowClearCmd(cr->displayList, cr->bg_color_.x, cr->bg_color_.y, cr->bg_color_.z, cr->bg_color_.w);

        addEnableDepthTest(cr->displayList);

        cr->ChangeShader(0);
        addShadowFrameBufferCmd(cr->displayList, cr->data->shaders[0].id, cr->data->shadowframebuffer->lightProjection, 2, cr->data->shadowframebuffer->FBtexture);

        addDrawSkybox(cr->displayList, cr->data->shaders[cr->cubemap->shader_idx].id, cr->data->localtrcmp[0].position,
          cr->data->cameracmp[0].front_, cr->data->cameracmp[0].Up,
          cr->data->cameracmp[0].projection_, cr->data->cameracmp[0].view_,
          cr->cubemap->vao, cr->cubemap->texture);
        cr->ExecSystems();

        addDisableDepthTest(cr->displayList);
        addDoFramebuffer(cr->displayList, cr->data->shaders[cr->data->framebuffer->shader_idx].id, cr->data->framebuffer->rectVAO, 
          cr->data->framebuffer->FBtexture, cr->data->framebuffer->FBOid, 
          cr->data->framebuffer->indices, cr->data->framebuffer->type);

			};
			auto schedShadowUpdate = [cr] {
				cr->ShadowMapUpdate();
			};

      

			float lastFrame = (float)glfwGetTime();
			InitImGUI();
			
      scheduler.waitFor(schedulerReady);

      while (!glfwWindowShouldClose(_window._window))
      {
        std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();

        if (glfwGetKey(_window._window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
          glfwSetWindowShouldClose(_window._window, GL_TRUE);
        
				deltaTime_ = (float)glfwGetTime() - lastFrame;
				lastFrame = (float)glfwGetTime();
        
				MoveCamera();
        ShadowMapUpdate();
        displayList->executeOnGPU();
     


        //////////////////TEST////////////////////////////////


        ChangeShader(data->silhoutteframebuffer->shader_idx);
        data->shaders[data->silhoutteframebuffer->shader_idx].Activate();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, kWidth - 300.0f, kHeight);
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, data->silhoutteframebuffer->FBOid);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindTextureUnit(0, data->silhoutteframebuffer->FBtexture);

        glUniform1i(glGetUniformLocation(data->shaders[data->silhoutteframebuffer->shader_idx].id, "silhoutteTexture"), 0);

        //ExecSystems();
        for (int i = 0; i < data->entities.size(); i++) {
          if ((data->entities[i]->componentsID_ & kSilhoutteComp) == kSilhoutteComp) {
            if ((data->entities[i]->componentsID_ & kRenderComp) == kRenderComp) {
              SilhoutteSystem silsys;
              silsys.exec(*data->entities[i], data, displayList);
            }
          }
        }

        data->shaders[data->silhoutteframebuffer->shader_idx].Activate();

        //glUniform3fv(glGetUniformLocation(data->shaders[data->silhoutteframebuffer->shader_idx].id, "color"), 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
        int tex = glGetUniformLocation(data->shaders[data->silhoutteframebuffer->shader_idx].id, "silhoutteTexture");
        int color = glGetUniformLocation(data->shaders[data->silhoutteframebuffer->shader_idx].id, "color");

        //glUniform3fv(glGetUniformLocation(data->shaders[data->silhoutteframebuffer->shader_idx].id, "color"), 1, glm::value_ptr(vec3(0.0f, 1.0f, 0.0)));
        
       
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(bg_color_.x, bg_color_.y, bg_color_.z, bg_color_.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        //////////////////////////////////////////////////////


        scheduler.run(schedUpdate, &schedulerUpdateReady);
        
        scheduler.waitFor(schedulerUpdateReady);
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
    addBindFramebuffer(displayList, 0);
    addViewPortCmd(displayList, 0, 0, kWidth - 300.0f, kHeight);
    addBindFramebuffer(displayList, data->framebuffer->FBOid);
    addWindowClearCmd(displayList, bg_color_.x, bg_color_.y, bg_color_.z, bg_color_.w);

    addEnableDepthTest(displayList);

    ChangeShader(0);

    addShadowFrameBufferCmd(displayList, data->shaders[0].id, data->shadowframebuffer->lightProjection, 2, data->shadowframebuffer->FBtexture);

    addDrawSkybox(displayList, data->shaders[cubemap->shader_idx].id, data->localtrcmp[0].position,
      data->cameracmp[0].front_, data->cameracmp[0].Up,
      data->cameracmp[0].projection_, data->cameracmp[0].view_,
      cubemap->vao, cubemap->texture);
    ExecSystems();

    addDisableDepthTest(displayList);
    addDoFramebuffer(displayList, data->shaders[data->framebuffer->shader_idx].id, 
      data->framebuffer->rectVAO, data->framebuffer->FBtexture, data->framebuffer->FBOid, 
     data->framebuffer->indices, data->framebuffer->type);

	}

  void Core::ShadowMapUpdate()
  {
    ChangeShader(data->shadowframebuffer->shader_idx);
    data->shaders[data->shadowframebuffer->shader_idx].Activate();

    addEnableDepthTest(displayList);
    addViewPortCmd(displayList, 0, 0, kWidth - 300.0f, kHeight);
    addBindFramebuffer(displayList, data->shadowframebuffer->FBOid);
    addClearDepthBufferCmd(displayList);
    ExecSystems();
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

  void Core::DeleteEntity(int index)
  {
    for (int i = 0; i < data->entities.size(); i++) {

      if (i != index) {

        if ((data->entities[i]->componentsID_ & kHeritageComp) == kHeritageComp) {
          if (data->entities[i]->cmp_indx_[kHeritageCompPos] < data->heritagecmp.size()) {

            if (data->heritagecmp[data->entities[i]->cmp_indx_[kHeritageCompPos]].parentID == data->entities[index]->entityIdx) {
              DeleteEntity(i);
              i = 0;
            }
          }
        }
      }
    }
    if ((data->entities[index]->componentsID_ & kLocalTRComp) == kLocalTRComp) {
      data->localtrcmp.erase(data->localtrcmp.begin() + data->entities[index]->cmp_indx_[kLocalTRCompPos]);
    }
    if ((data->entities[index]->componentsID_ & kCameraComp) == kCameraComp) {
      data->cameracmp.erase(data->cameracmp.begin() + data->entities[index]->cmp_indx_[kCameraCompPos]);
    }
    if ((data->entities[index]->componentsID_ & kRenderComp) == kRenderComp) {
      data->rendercmp.erase(data->rendercmp.begin() + data->entities[index]->cmp_indx_[kRenderCompPos]);
    }
    if ((data->entities[index]->componentsID_ & kHeritageComp) == kHeritageComp) {
      data->heritagecmp.erase(data->heritagecmp.begin() + data->entities[index]->cmp_indx_[kHeritageCompPos]);
    }
    if ((data->entities[index]->componentsID_ & kLightComp) == kLightComp) {
      data->lightcmp.erase(data->lightcmp.begin() + data->entities[index]->cmp_indx_[kLightCompPos]);
    }
    if ((data->entities[index]->componentsID_ & kTypeLightComp) == kTypeLightComp) {
      data->typelighcmp.erase(data->typelighcmp.begin() + data->entities[index]->cmp_indx_[kTypeLightCompPos]);
    }

    for (int i = 0; i < data->entities.size(); i++) {

      if (i > index) {
        if ((data->entities[index]->componentsID_ & kLocalTRComp) == kLocalTRComp &&
          (data->entities[i]->componentsID_ & kLocalTRComp) == kLocalTRComp) {
          data->entities[i]->cmp_indx_[kLocalTRCompPos] -= 1;
        }
        if ((data->entities[index]->componentsID_ & kCameraComp) == kCameraComp &&
          (data->entities[i]->componentsID_ & kCameraComp) == kCameraComp) {
          data->entities[i]->cmp_indx_[kCameraCompPos] -= 1;
        }
        if ((data->entities[index]->componentsID_ & kRenderComp) == kRenderComp &&
          (data->entities[i]->componentsID_ & kRenderComp) == kRenderComp) {
          data->entities[i]->cmp_indx_[kRenderCompPos] -= 1;
        }
        if ((data->entities[index]->componentsID_ & kHeritageComp) == kHeritageComp &&
          (data->entities[i]->componentsID_ & kHeritageComp) == kHeritageComp) {
          data->entities[i]->cmp_indx_[kHeritageCompPos] -= 1;
        }
        if ((data->entities[index]->componentsID_ & kLightComp) == kLightComp &&
          (data->entities[i]->componentsID_ & kLightComp) == kLightComp) {
          data->entities[i]->cmp_indx_[kLightCompPos] -= 1;
        }
        if ((data->entities[index]->componentsID_ & kTypeLightComp) == kTypeLightComp &&
          (data->entities[i]->componentsID_ & kTypeLightComp) == kTypeLightComp) {
          data->entities[i]->cmp_indx_[kTypeLightCompPos] -= 1;
        }

      }

      
    }



    data->entities.erase(data->entities.begin() + index);

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
            data->heritagecmp.push_back(HeritageComponent());
            entity.cmp_indx_[kHeritageCompPos] = (int)(data->heritagecmp.size() - 1);
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
				case kSilhoutteComp: {
					data->silhouttecmp.push_back(SilhoutteComponent());
					entity.cmp_indx_[kSilhoutteCompPos] = (int)(data->silhouttecmp.size() - 1);
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
    if ((components & kCameraComp) == kCameraComp) {
      data->cameracmp.push_back(CameraComponent());
      entity.cmp_indx_[kCameraCompPos] = (int)(data->cameracmp.size() - 1);
    }
    if ((components & kRenderComp) == kRenderComp) {
      data->rendercmp.push_back(RenderComponent());
      entity.cmp_indx_[kRenderCompPos] = (int)(data->rendercmp.size() - 1);
    }
    if ((components & kHeritageComp) == kHeritageComp) {
      data->heritagecmp.push_back(HeritageComponent());
      entity.cmp_indx_[kHeritageCompPos] = (int)(data->heritagecmp.size() - 1);
    }
    if ((components & kLightComp) == kLightComp) {
      data->lightcmp.push_back(LightComponent());
      entity.cmp_indx_[kLightCompPos] = (int)(data->lightcmp.size() - 1);
    }
    if ((components & kTypeLightComp) == kTypeLightComp) {
      data->typelighcmp.push_back(TypeLightComponent());
      entity.cmp_indx_[kTypeLightCompPos] = (int)(data->typelighcmp.size() - 1);
    }
    if ((components & kSilhoutteComp) == kSilhoutteComp) {
      data->silhouttecmp.push_back(SilhoutteComponent());
      entity.cmp_indx_[kSilhoutteCompPos] = (int)(data->silhouttecmp.size() - 1);
    }
  }

  void Core::SetChildren(Entity& entity, GLuint parentIndex)
  {
    data->heritagecmp.push_back(HeritageComponent());
    data->heritagecmp.back().parentID = parentIndex;
    entity.cmp_indx_[kHeritageCompPos] = data->heritagecmp.size() - 1;
  }

  void Core::AddChildren(GLuint parentIndex)
  {
    int index_entity = AddEntity();

    AddComponent(*data->entities[index_entity], kLocalTRComp);
    AddComponent(*data->entities[index_entity], kLightComp);
    AddComponent(*data->entities[index_entity], kRenderComp);
    AddComponent(*data->entities[index_entity], kHeritageComp);
    data->heritagecmp[data->entities[index_entity]->cmp_indx_[kHeritageCompPos]].parentID = parentIndex;

    PreExecSystem(*data->entities[index_entity]);
  }

	Material Core::InitMaterial(vector<string> texturePaths)
	{
    Material tmpMat;
		
		for (int i = 0; i < texturePaths.size(); i++) {
			if(i == 0)
        tmpMat.textures.push_back(Texture(texturePaths[i].c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE));
			else if(i == 1)
        tmpMat.textures.push_back(Texture(texturePaths[i].c_str(), "specular", 1, GL_RED, GL_UNSIGNED_BYTE));
			else if(i  == 2)
        tmpMat.textures.push_back(Texture(texturePaths[i].c_str(), "normal", 2, GL_RGB, GL_UNSIGNED_BYTE));
		}
		return tmpMat;
	}

	Geometry Core::InitGeometry(const char* path)
	{
		Geometry geo;
		loadOBJ2(path, geo);

		return geo;
	}

	void Core::ChangeMesh(Entity& entity, RenderCtx* data, int option)
	{
		data->rendercmp[entity.cmp_indx_[kRenderCompPos]].mesh_idx[0] = option;
	}
  void Core::ChangeMaterial(Entity& entity, RenderCtx* data, int option) {
    data->rendercmp[entity.cmp_indx_[kRenderCompPos]].material_idx[0] = option;
  }
  void Core::ClearDataLists(vector<std::vector<int>>* selectedType)
  {
    data->entities.erase(data->entities.begin() + 1, data->entities.end());
    data->localtrcmp.erase(data->localtrcmp.begin() + 1, data->localtrcmp.end());
    data->rendercmp.clear();
    data->lightcmp.clear();
    data->typelighcmp.clear();
    selectedType->clear();
  }

  void Core::InitMesh(string geometryPath)
	{
    Geometry geo;
    loadOBJ2(geometryPath.c_str(), geo);
		Mesh newMesh(geo.totalVertex_, geo.totalIndices_, geometryPath, geo);
		data->meshes.push_back(newMesh);
   
	}


	void Core::InitSystems()
	{
		data->sys.push_back(make_shared<LocalTRSystem>());
		data->sys.push_back(make_shared<CameraSystem>());
		data->sys.push_back(make_shared<RenderSystem>());
		data->sys.push_back(make_shared<HeritageSystem>());
		data->sys.push_back(make_shared<LightSystem>());
		data->sys.push_back(make_shared<TypeLightSystem>());
		data->sys.push_back(make_shared<SilhoutteSystem>());
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

  void EditTransform(const Utlop::CameraComponent& camera, LocalTRComponent& transform, int n, int hasHeritageCmp)
  {
    ImGuizmo::BeginFrame();
    
    std::string slidername;
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);

    
    vec3 position;
    vec3 rotation;
    vec3 scale;

    if (ImGui::IsKeyPressed(ImGuiKey_T))
      mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R))
      mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_Y)) 
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
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x - 300.0f, io.DisplaySize.y);
    ImGuizmo::Manipulate(value_ptr(camera.view_), value_ptr(camera.projection_),
      mCurrentGizmoOperation, mCurrentGizmoMode, value_ptr(modelMat),
      NULL, nullptr);

    transform.model = modelMat;

    if(hasHeritageCmp == -1)
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

    static int gameObjectSelected = -1;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(300, 780));
    static vector<std::vector<int>> selectedType;
    for (int i = 0; i < data->entities.size(); i++) {
      if (i == selectedType.size()) {
        selectedType.push_back(vector<int>(0));
      }
    }
    static vector<std::vector<int>> selectedMaterial;
    for (int i = 0; i < data->entities.size(); i++) {
      if (i == selectedMaterial.size()) {
        selectedMaterial.push_back(vector<int>(0));
      }
    }
		if (ImGui::Begin("Utlop Engine")) {

      ImGui::TextColored(ImVec4(1, 0, 0, 1), "Postprocess");
      ImGui::RadioButton("Without", &data->framebuffer->type, 0); ImGui::SameLine();
      ImGui::RadioButton("Silhoutte", &data->framebuffer->type, 1); ImGui::SameLine();
      ImGui::RadioButton("Blurr", &data->framebuffer->type, 2);

			if (ImGui::Button("Add Entity")) {
				int entityIdx = AddEntity();
				AddComponent(*data->entities[entityIdx], kLocalTRComp);
				AddComponent(*data->entities[entityIdx], kRenderComp);
				AddComponent(*data->entities[entityIdx], kLightComp);
				PreExecSystem(*data->entities[entityIdx]);
			} ImGui::SameLine();
			if (ImGui::Button("Entity Silhoutte")) {
				int entityIdx = AddEntity();
				AddComponent(*data->entities[entityIdx], kLocalTRComp);
				AddComponent(*data->entities[entityIdx], kRenderComp);
        AddComponent(*data->entities[entityIdx], kLightComp);
				AddComponent(*data->entities[entityIdx], kSilhoutteComp);
				PreExecSystem(*data->entities[entityIdx]);
			} ImGui::SameLine();

      if (ImGui::Button("Load DataBase")) {
        gameObjectSelected = -1;
        DataBase db;
        ClearDataLists(&selectedType);
        db.LoadDatabase("../UtlopTests/src/db/scene.db", data);
        PreExecSystems();
      }

			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Current GameObjects");
      if(ImGui::BeginChild("GameObject")) {


        const char* obj_type[]{ "Robot", "White Cube", "Helmet", "Container", "Car", "Cube" };

        if (ImGui::TreeNode("Objects"))
        {
          for (int i = 0; i < data->entities.size(); i++)
          {
            // Use SetNextItemOpen() so set the default state of a node to be open. We could
            // also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
            if (i == 0)
              ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            std::string name;
            if (data->entities[i]->cmp_indx_[kTypeLightCompPos] != -1) {
              name = "Light " + std::to_string(i);
            }
            else if (data->entities[i]->cmp_indx_[kCameraCompPos] != -1) {
              name = "Camera " + std::to_string(i);
            }
            else {
              name = "Game object " + std::to_string(i);
            }
            if (ImGui::TreeNode((void*)(intptr_t)i, name.c_str(), i))
            {
              if (ImGui::SmallButton("Properties")) {
                gameObjectSelected = i;
              }
              ImGui::TreePop();
            }
          }
          ImGui::TreePop();
        }

        
        ImGui::EndChild();

      }

      ImGui::SetNextWindowPos(ImVec2(1380, 0));
      ImGui::SetNextWindowSize(ImVec2(300, 780));
      if (ImGui::Begin("Properties")) {

        if (gameObjectSelected > -1) {


          vec3 position;
          vec3 rotation;
          vec3 scale;
          
          if (data->entities[gameObjectSelected]->cmp_indx_[kLocalTRCompPos] != -1) {

              ImGui::Text("%04d: Object", gameObjectSelected);

              EditTransform(data->cameracmp[0], data->localtrcmp[data->entities[gameObjectSelected]->cmp_indx_[kLocalTRCompPos]], gameObjectSelected, data->entities[gameObjectSelected]->cmp_indx_[kHeritageCompPos]);

              position = data->localtrcmp[data->entities[gameObjectSelected]->cmp_indx_[kLocalTRCompPos]].position;
              rotation = data->localtrcmp[data->entities[gameObjectSelected]->cmp_indx_[kLocalTRCompPos]].rotation;
              scale = data->localtrcmp[data->entities[gameObjectSelected]->cmp_indx_[kLocalTRCompPos]].scale;
              ImGui::SetCursorPosX(100.0f);
              
              std::string slidername = "Location of " + std::to_string(gameObjectSelected);
              ImGui::Text("Location");
              ImGui::SliderFloat3(slidername.c_str(), &position[0], -100.0f, 100.0f);
              data->localtrcmp[data->entities[gameObjectSelected]->cmp_indx_[kLocalTRCompPos]].position = position;
              ImGui::SetCursorPosX(100.0f);
              std::string slidername2 = "Rotation of " + std::to_string(gameObjectSelected);
              ImGui::Text("Rotation");
              ImGui::SliderFloat3(slidername2.c_str(), &rotation[0], -359.0f, 359.0f);
              data->localtrcmp[data->entities[gameObjectSelected]->cmp_indx_[kLocalTRCompPos]].rotation = rotation;
              ImGui::SetCursorPosX(100.0f);
              std::string slidername3 = "Scale of " + std::to_string(gameObjectSelected);
              ImGui::Text("Scale");
              ImGui::SliderFloat3(slidername3.c_str(), &scale[0], 0.0f, 10.0f);
              data->localtrcmp[data->entities[gameObjectSelected]->cmp_indx_[kLocalTRCompPos]].scale = scale;
            }
            if (data->entities[gameObjectSelected]->cmp_indx_[kRenderCompPos] != -1 && data->entities[gameObjectSelected]->cmp_indx_[kTypeLightCompPos] == -1) {
              char s[10];
              itoa(gameObjectSelected, s, 10);
              selectedType[gameObjectSelected].push_back(data->rendercmp[data->entities[gameObjectSelected]->cmp_indx_[kRenderCompPos]].mesh_idx[0]);
              int tmpSelected = selectedType[gameObjectSelected][0];
              ImGui::ListBox(s, &selectedType[gameObjectSelected][0], &data->obj_str_type[0], data->obj_str_type.size());
              if(tmpSelected != selectedType[gameObjectSelected][0])
                ChangeMesh(*data->entities[gameObjectSelected], data, selectedType[gameObjectSelected][0]);
              
              char m[10];
              itoa(gameObjectSelected+100, m, 10);
              selectedMaterial[gameObjectSelected].push_back(data->rendercmp[data->entities[gameObjectSelected]->cmp_indx_[kRenderCompPos]].material_idx[0]);
              int tmpmatSelected = selectedMaterial[gameObjectSelected][0];
              ImGui::ListBox(m, &selectedMaterial[gameObjectSelected][0], &data->mat_str_type[0], data->mat_str_type.size());
              if(tmpmatSelected != selectedMaterial[gameObjectSelected][0])
                ChangeMaterial(*data->entities[gameObjectSelected], data, selectedMaterial[gameObjectSelected][0]);
            
              if (ImGui::SmallButton("Delete Entity")) {
                DeleteEntity(gameObjectSelected);
                gameObjectSelected = -1;
              }
            }
            else if (data->entities[gameObjectSelected]->cmp_indx_[kTypeLightCompPos] != -1) {
              static int e = 0;
              std::string radioname = "PointLight " + std::to_string(gameObjectSelected);
              ImGui::RadioButton(radioname.c_str(), &data->typelighcmp[data->entities[gameObjectSelected]->cmp_indx_[kTypeLightCompPos]].type, 0); ImGui::SameLine();
              radioname = "DirectLight " + std::to_string(gameObjectSelected);
              ImGui::RadioButton(radioname.c_str(), &data->typelighcmp[data->entities[gameObjectSelected]->cmp_indx_[kTypeLightCompPos]].type, 1); ImGui::SameLine();
              radioname = "SpotLight " + std::to_string(gameObjectSelected);
              ImGui::RadioButton(radioname.c_str(), &data->typelighcmp[data->entities[gameObjectSelected]->cmp_indx_[kTypeLightCompPos]].type, 2);
              if (ImGui::SmallButton("Delete Entity")) {
                DeleteEntity(gameObjectSelected);
                gameObjectSelected = -1;
              }
            }

            if (ImGui::SmallButton("Add Children")) {
              AddChildren(gameObjectSelected);
            }
        }
        ImGui::End();
      }
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
