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

unsigned int loadCubemap(std::vector<std::string> faces);
void loadVertexShader(const char* filename, Utlop::RenderComponent& rc);
void loadFragmentShader(const char* filename, Utlop::RenderComponent& rc);
void setMat4fv(GLuint shader_id, glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);

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
		AddEntity();
		AddComponent(*data->entities[0], kCameraComp);
		AddComponent(*data->entities[0], kLocalTRComp);
		//AddComponent(*data->entities[0], kRenderComp);
		
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 10; j++) {
				int entityIdx = AddEntity();
				AddComponent(*data->entities[entityIdx], kLocalTRComp);
				AddComponent(*data->entities[entityIdx], kRenderComp);
				data->localtrcmp[data->entities[entityIdx]->cmp_indx_[kLocalTRCompPos]].position -= vec3(3.0f * i, 3.0f * j, (rand()%10) - 5.0f);
			}
		}
		
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
			AddCubeMap();

			PreExecSystems();

      while (!glfwWindowShouldClose(_window._window))
      {
        std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();

        if (glfwGetKey(_window._window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
          glfwSetWindowShouldClose(_window._window, GL_TRUE);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				deltaTime_ = (float)glfwGetTime() - lastFrame;
				lastFrame = (float)glfwGetTime();

				glfwPollEvents();

				/*glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
				glUseProgram(data->cubemap.shaderID_);
				glm::mat4 view = glm::mat4(glm::mat3(data->cameracmp[0].view_)); // remove translation from the view matrix
				setMat4fv(data->cubemap.shaderID_, view, "ViewMatrix");
				setMat4fv(data->cubemap.shaderID_, data->cameracmp[0].projection_, "ProjectionMatrix");
				// skybox cube
				glBindVertexArray(data->cubemap.vao_);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, data->cubemap.material_idx[0]);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0);
				glDepthFunc(GL_LESS); // set depth function back to default
				*/

				ExecSystems();
				MoveCamera();

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
		newEntity.cmp_indx_.push_back(-1);
		newEntity.cmp_indx_.push_back(-1);
		newEntity.cmp_indx_.push_back(-1);
		newEntity.cmp_indx_.push_back(-1);
		

		data->entities.push_back(make_shared<Entity>(newEntity));
		return data->entities.size() - 1;
	}

	void Core::AddComponent(Entity& entity, Utlop::ComponentID id)
	{
		if ((entity.componentsID_ & id) == 0) {

			entity.AddComponent(id);

			switch (id) {
				case kLocalTRComp: {
						data->localtrcmp.push_back(LocalTRComponent());
						entity.cmp_indx_[kLocalTRCompPos] = data->localtrcmp.size() - 1;
					break;
				}
				case kWorldTRComp: {
						data->worldtrcmp.push_back(WorldTRComponent());
						entity.cmp_indx_[kWorldTRCompPos] = data->worldtrcmp.size() - 1;
					break;
				}
				case kCameraComp: {
						data->cameracmp.push_back(CameraComponent());
						entity.cmp_indx_[kCameraCompPos] = data->cameracmp.size() - 1;
					break;
				}
				case kRenderComp: {
						data->rendercmp.push_back(RenderComponent());
						entity.cmp_indx_[kRenderCompPos] = data->rendercmp.size() - 1;
					break;
				}
			}

		}
		else {
			printf("\nComponent already in Entity\n");
		}
	
	}

	void Core::AddCubeMap()
	{
		vector<std::string> faces
		{
				"../UtlopTests/src/textures/cubemap/right.jpg",
				"../UtlopTests/src/textures/cubemap/left.jpg",
				"../UtlopTests/src/textures/cubemap/top.jpg",
				"../UtlopTests/src/textures/cubemap/bottom.jpg",
				"../UtlopTests/src/textures/cubemap/front.jpg",
				"../UtlopTests/src/textures/cubemap/back.jpg"
		};

		data->cubemap.shaderID_ = glCreateProgram();
		data->cubemap.material_idx.push_back(loadCubemap(faces));

		

		loadVertexShader("../UtlopTests/src/shaders/vs.glsl", data->cubemap);
		loadFragmentShader("../UtlopTests/src/shaders/fs_cubemap.glsl", data->cubemap);

		glLinkProgram(data->cubemap.shaderID_);
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		glGenVertexArrays(1, &data->cubemap.vao_);
		glGenBuffers(1, &data->cubemap.vbo_);
		glBindVertexArray(data->cubemap.vao_);
		glBindBuffer(GL_ARRAY_BUFFER, data->cubemap.vbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


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

	void Core::PreExecSystems()
	{
		for (unsigned int i = 0; i < data->sys.size(); i++) {
			for (unsigned int h = 0; h < data->entities.size(); h++) {
				int out = data->entities[h]->componentsID_ & data->sys[i]->id_;
				if (out == data->sys[i]->id_)
					data->sys[i]->preExec(*data->entities[h], data);
			}
		}
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
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			Utlop::Core::Instance()->polygon_ = !Utlop::Core::Instance()->polygon_;
		}
	}

}
