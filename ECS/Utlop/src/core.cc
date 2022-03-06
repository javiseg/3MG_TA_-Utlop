#include "core.h"

#include <chrono>
#include <thread>
#include <fstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "..\include\core.h"
#include "imguiStruct.h"
#include "system.h"
#include <time.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

namespace Utlop
{
  Core* Core::_instance = nullptr;

  Core::Core()
  {
    _fps = 0.0f;
    _frame_time_millis = 0;
		data = new RenderCtx();
    _instance = this;

  }

  Core::~Core()
  {
		delete data;
		DestroyImGUI();
		glfwTerminate();
  }

  bool Core::init(float fps)
  {
    _fps = fps;
    _frame_time_millis = 1000 / (long)_fps;

		//camera_.alloc();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		polygon_ = false;
		camera_speed_ = 10.0f;
		InitSystems();
		InitComponents();

		//Debug:
		AddEntity(true);
		AddEntity(false);
		AddEntity(false);
		data->localtrcmp[data->entities[2]->cmp_indx_[0]].position = vec3(-3.0f, 0.0f, 0.0f);
		//AddEntity();
		
		bg_color_ = vec3(0.0f);

		

    return glfwInit();
  }

  void Core::start()
  {
    if (_window.isValid())
    {
      glfwMakeContextCurrent(_window._window);

      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      {
        printf("Failed to initialize GLAD");
      }


			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glClearColor(160.0f/255.0f, 160.0f / 255.0f, 160.0f / 255.0f, 1.0f);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			GLint version_max, version_min;
			glGetIntegerv(GL_MAJOR_VERSION, &version_max);
			glGetIntegerv(GL_MINOR_VERSION, &version_min);
			printf("Version: %d.%d \n", version_max, version_min);

			float lastFrame = (float)glfwGetTime();
			InitImGUI();

      while (!glfwWindowShouldClose(_window._window))
      {
        std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();

        if (glfwGetKey(_window._window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
          glfwSetWindowShouldClose(_window._window, GL_TRUE);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				deltaTime_ = (float)glfwGetTime() - lastFrame;
				lastFrame = (float)glfwGetTime();

				glfwPollEvents();

				ImGUI();

				ExecSystems();
				MoveCamera();

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

  Utlop::Window* Core::getWindow()
  {
    return &_window;
  }

	float Core::getDeltaTime()
	{
		return deltaTime_;
	}

	void Core::AddEntity(bool camera)
	{
		Entity newEntity;

		newEntity.AddComponent(kLocalTRComp);
		data->localtrcmp.push_back(LocalTRComponent());

		newEntity.AddComponent(kWorldTRComp);
		data->worldtrcmp.push_back(WorldTRComponent());

		if (camera) {

			newEntity.AddComponent(kCameraComp);
			data->cameracmp.push_back(CameraComponent());
			data->localtrcmp[data->localtrcmp.size() -1].position = vec3(0.0f, 0.0f, 5.0f);
		}
		else {

			newEntity.AddComponent(kRenderComp);
			data->rendercmp.push_back(RenderComponent());
		}



		
		int offset = 0;
		if (newEntity.componentsID_ & 1) {
			newEntity.cmp_indx_.push_back(data->localtrcmp.size() - 1);
			offset++;
		}
		else {
			newEntity.cmp_indx_.push_back(-1);
		}
		if (newEntity.componentsID_ & 2) {
			newEntity.cmp_indx_.push_back(data->worldtrcmp.size() - 1);
			offset++;
		}
		else {
			newEntity.cmp_indx_.push_back(-1);
		}
		if (newEntity.componentsID_ & 4) {
			newEntity.cmp_indx_.push_back(data->cameracmp.size() - 1);
			offset++;
		}
		else {
			newEntity.cmp_indx_.push_back(-1);
		}
		if (newEntity.componentsID_ & 8) {
			newEntity.cmp_indx_.push_back(data->rendercmp.size() - 1);
			offset++;
		}
		else {
			newEntity.cmp_indx_.push_back(-1);
		}
		

		data->entities.push_back(make_shared<Entity>(newEntity));
	}

	void Core::MoveCamera()
	{
		glfwSetKeyCallback(getWindow()->getWindow(), key_callback);
	}

	void Core::InitComponents()
	{
		data->kComponentMap.insert(make_pair(kLocalTRComp, LocalTRComponent()));
		data->kComponentMap.insert(make_pair(kWorldTRComp, WorldTRComponent()));
		data->kComponentMap.insert(make_pair(kRenderComp, RenderComponent()));
		data->kComponentMap.insert(make_pair(kCameraComp, CameraComponent()));
	}

	void Core::InitSystems()
	{
		data->sys.push_back(make_shared<LocalTRSystem>());
		data->sys.push_back(make_shared<WorldTRSystem>());
		data->sys.push_back(make_shared<CameraSystem>());
		data->sys.push_back(make_shared<RenderSystem>());
	}

	void Core::ExecSystems()
	{
		for (unsigned int i = 0; i < data->sys.size(); i++) {
			for (unsigned int h = 0; h < data->entities.size(); h++) {
				int out = data->entities[h]->componentsID_ & data->sys[i]->id_;
				if (out == data->sys[i]->id_)
					data->sys[i]->exec(*data->entities[h], data);
			}
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
	float Core::getCameraSpeed()
	{
		return camera_speed_;
	}
	void Core::ImGUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(300, 780));
		if (ImGui::Begin("Utlop Engine")) {

			ImGui::ColorEdit4("Color", &bg_color_[0]);
			glClearColor(bg_color_.x, bg_color_.y, bg_color_.z, 1.0f);

		
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
}






void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W) {
		Utlop::Core::Instance()->getData()->localtrcmp[0].position +=
			Utlop::Core::Instance()->getData()->cameracmp[0].front_
			* Utlop::Core::Instance()->getCameraSpeed() * Utlop::Core::Instance()->getDeltaTime();
	}
	if (key == GLFW_KEY_S) {
		Utlop::Core::Instance()->getData()->localtrcmp[0].position -=
			Utlop::Core::Instance()->getData()->cameracmp[0].front_
			* Utlop::Core::Instance()->getCameraSpeed() * Utlop::Core::Instance()->getDeltaTime();
	}
	
	if (key == GLFW_KEY_D) {
		Utlop::Core::Instance()->getData()->localtrcmp[0].position +=
			Utlop::Core::Instance()->getData()->cameracmp[0].Right *
			Utlop::Core::Instance()->getCameraSpeed() * Utlop::Core::Instance()->getDeltaTime();
	}
	
	if (key == GLFW_KEY_A) {
		Utlop::Core::Instance()->getData()->localtrcmp[0].position -=
			Utlop::Core::Instance()->getData()->cameracmp[0].Right *
			Utlop::Core::Instance()->getCameraSpeed() * Utlop::Core::Instance()->getDeltaTime();
	}
	if (key == GLFW_KEY_Q) {
		Utlop::Core::Instance()->getData()->localtrcmp[0].position +=
			Utlop::Core::Instance()->getData()->cameracmp[0].Up *
			Utlop::Core::Instance()->getCameraSpeed() * Utlop::Core::Instance()->getDeltaTime();
	}
	if (key == GLFW_KEY_E) {
		Utlop::Core::Instance()->getData()->localtrcmp[0].position -=
			Utlop::Core::Instance()->getData()->cameracmp[0].Up *
			Utlop::Core::Instance()->getCameraSpeed() * Utlop::Core::Instance()->getDeltaTime();
	}
	
	if (key == GLFW_KEY_J) {
		Utlop::Core::Instance()->getData()->cameracmp[0].yaw_ -= Utlop::Core::Instance()->getCameraSpeed() * 20.0f * Utlop::Core::Instance()->getDeltaTime();
	}
	if (key == GLFW_KEY_L) {
		Utlop::Core::Instance()->getData()->cameracmp[0].yaw_ += Utlop::Core::Instance()->getCameraSpeed() * 20.0f * Utlop::Core::Instance()->getDeltaTime();
	}
	if (key == GLFW_KEY_I) {
		Utlop::Core::Instance()->getData()->cameracmp[0].pitch_ += Utlop::Core::Instance()->getCameraSpeed() * 20.0f * Utlop::Core::Instance()->getDeltaTime();
	}
	if (key == GLFW_KEY_K) {
		Utlop::Core::Instance()->getData()->cameracmp[0].pitch_ -= Utlop::Core::Instance()->getCameraSpeed() * 20.0f * Utlop::Core::Instance()->getDeltaTime();
	}
	
	
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		if (Utlop::Core::Instance()->polygon_) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		Utlop::Core::Instance()->polygon_ = !Utlop::Core::Instance()->polygon_;
	}
}