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
#include "stb_image.h"
#include "stb_image_write.h"
#define PX_SCHED_IMPLEMENTATION 1
#include "px_sched.h"

void loadCubemap(const char* path, GLuint& texture);
void loadVertexShader(const char* filename, Utlop::RenderCtx* data);
void loadFragmentShader(const char* filename, Utlop::RenderCtx* data);
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

  }

  Core::~Core()
  {
		delete data;
		delete displayList;
		DestroyImGUI();
		glfwTerminate();
  }

	void Utlop::Core::createEntities(Core* cr) {
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				int entityIdx = cr->AddEntity();
				cr->AddComponent(*cr->getData()->entities[entityIdx], kLocalTRComp);
				cr->AddComponent(*cr->getData()->entities[entityIdx], kRenderComp);
				cr->getData()->localtrcmp[cr->getData()->entities[entityIdx]->cmp_indx_[kLocalTRCompPos]].position -= vec3(30.0f * i, 30.0f * j, (rand() % 10) - 5.0f);
			}
		}
	}
  bool Core::init(float fps)
  {
    _fps = fps;
    _frame_time_millis = 1000 / (long)_fps;
		preExecDone_ = 0;
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
		data->localtrcmp[data->entities[0]->cmp_indx_[kCameraCompPos]].position = vec3(0.0f, 0.0f, 55.0f);
		//AddComponent(*data->entities[0], kRenderComp);
		
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
		

    return glfwInit();
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
				//printf("Failed to execute\n");
				cr->PreExecSystems();
			};
			auto sched = [cr] {
				//printf("Failed to execute\n");
				cr->ExecSystems();
			};


      glfwMakeContextCurrent(_window._window);

      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      {
        printf("Failed to initialize GLAD");
      }
			
			//Add Vertex
			loadVertexShader("../UtlopTests/src/shaders/vs.glsl", data);
			loadFragmentShader("../UtlopTests/src/shaders/fs_texture.glsl", data);
			/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glClearColor(160.0f/255.0f, 160.0f / 255.0f, 160.0f / 255.0f, 1.0f);*/


			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			GLint version_max, version_min;
			glGetIntegerv(GL_MAJOR_VERSION, &version_max);
			glGetIntegerv(GL_MINOR_VERSION, &version_min);
			printf("Version: %d.%d \n", version_max, version_min);

			float lastFrame = (float)glfwGetTime();
			InitImGUI();

			//PreExecSystems();
			InitMaterials(data, "../UtlopTests/src/textures/texture.jpg");
			InitMaterials(data, "../UtlopTests/src/textures/default.png");
			InitGeometry(data, "../UtlopTests/src/obj/robot.obj");
			//scheduler.run(preSched, &schedulerReady);
			PreExecSystems();

      while (!glfwWindowShouldClose(_window._window))
      {
        std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();

        if (glfwGetKey(_window._window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
          glfwSetWindowShouldClose(_window._window, GL_TRUE);

				addWindowClearCmd(displayList, bg_color_.x, bg_color_.y, bg_color_.z, bg_color_.w);
				
				deltaTime_ = (float)glfwGetTime() - lastFrame;
				lastFrame = (float)glfwGetTime();

				glfwPollEvents();


				//ExecSystems();
				if (preExecDone_) {
					scheduler.run(sched, &schedulerReady);
					ExecSystems2();
				}
					
				


				MoveCamera();

				/*const GLsizeiptr kUniformBufferSize = sizeof(PerFrameData);
				glUseProgram(data->cubemap.shaderID_);
				GLuint perFrameDataBuffer;
				glCreateBuffers(1, &perFrameDataBuffer);
				glNamedBufferStorage(perFrameDataBuffer, kUniformBufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
				glBindBufferRange(GL_UNIFORM_BUFFER, 0, perFrameDataBuffer, 0, kUniformBufferSize);

				const mat4 m = glm::scale(mat4(1.0f), vec3(2.0f));
				PerFrameData perFrameData = { perFrameData.model = m, perFrameData.mvp = data->cameracmp[0].view_ * data->cameracmp[0].projection_ * m, perFrameData.cameraPos = vec4(0.0f) };
				glNamedBufferSubData(perFrameDataBuffer, 0, kUniformBufferSize, &perFrameData);
				glUseProgram(data->cubemap.shaderID_);
				glDrawArrays(GL_TRIANGLES, 0, 36);*/
				
		

				displayList->submit();

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
		for (int i = 0; i < kMaxComponents; i++) {
			newEntity.cmp_indx_[i] = -1;
		}		

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

		const char* path = "../UtlopTests/src/textures/cubemap/plaza/piazza.hdr";

		data->cubemap.shaderID_ = glCreateProgram();
		//loadVertexShader("../UtlopTests/src/shaders/vs_cubemap.glsl", data->cubemap);
		//loadFragmentShader("../UtlopTests/src/shaders/fs_cubemap.glsl", data->cubemap);

		GLuint textureID;
		loadCubemap(path, textureID);
		data->cubemap.material_idx.push_back(textureID);

		



		glLinkProgram(data->cubemap.shaderID_);
		float skyboxVertices[] = {
			// positions          
			-0.5f,0.5f,-0.5f,
								-0.5f,-0.5f,-0.5f,
								0.5f,-0.5f,-0.5f,
								0.5f,0.5f,-0.5f,

								-0.5f,0.5f,0.5f,
								-0.5f,-0.5f,0.5f,
								0.5f,-0.5f,0.5f,
								0.5f,0.5f,0.5f,

								0.5f,0.5f,-0.5f,
								0.5f,-0.5f,-0.5f,
								0.5f,-0.5f,0.5f,
								0.5f,0.5f,0.5f,

								-0.5f,0.5f,-0.5f,
								-0.5f,-0.5f,-0.5f,
								-0.5f,-0.5f,0.5f,
								-0.5f,0.5f,0.5f,

								-0.5f,0.5f,0.5f,
								-0.5f,0.5f,-0.5f,
								0.5f,0.5f,-0.5f,
								0.5f,0.5f,0.5f,

								-0.5f,-0.5f,0.5f,
								-0.5f,-0.5f,-0.5f,
								0.5f,-0.5f,-0.5f,
								0.5f,-0.5f,0.5f
		};
		std::vector<int> dest;
		Geometry cubeGeo;
		cubeGeo.vertices_.insert(cubeGeo.vertices_.begin(),std::begin(skyboxVertices), std::end(skyboxVertices));
		data->geometry.push_back(cubeGeo);
		data->cubemap.geo_idx.push_back(data->geometry.size() - 1);

		glGenVertexArrays(1, &data->cubemap.vao_);
		glGenBuffers(1, &data->cubemap.vbo_);
		glBindVertexArray(data->cubemap.vao_);
		glBindBuffer(GL_ARRAY_BUFFER, data->cubemap.vbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
	}

	void Core::InitMaterials(RenderCtx* data, const char* path)
	{
		Material tmpText;
		tmpText.path_ = path;

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


			stbi_image_free(text_buffer_);

			data->material.push_back(tmpText);

		}
	}

	void Core::InitGeometry(RenderCtx* data, const char* path)
	{
		Geometry geo;
		loadOBJ2(path, geo);
		data->geometry.push_back(geo);

		glCreateVertexArrays(1, &data->rendercmp[0].vao_);
		glCreateVertexArrays(1, &data->rendercmp[0].nvao_);

		glCreateBuffers(1, &data->rendercmp[0].vbo_);
		glCreateBuffers(1, &data->rendercmp[0].ebo_);

		glNamedBufferData(data->rendercmp[0].vbo_,
			geo.vertices_.size() * sizeof(float),
			geo.vertices_.data(), GL_STATIC_DRAW);


		glNamedBufferData(data->rendercmp[0].ebo_,
			geo.verticesIndices_.size() * sizeof(GLuint),
			geo.verticesIndices_.data(), GL_STATIC_DRAW);

		glCreateBuffers(1, &data->rendercmp[0].nbo_);
		glCreateBuffers(1, &data->rendercmp[0].enbo_);

		glNamedBufferData(data->rendercmp[0].nbo_,
			geo.normals_.size() * sizeof(float),
			geo.normals_.data(), GL_STATIC_DRAW);


		glNamedBufferData(data->rendercmp[0].enbo_,
			geo.normalsIndices_.size() * sizeof(GLuint),
			geo.normalsIndices_.data(), GL_STATIC_DRAW);


		//Enable:

		glEnableVertexArrayAttrib(data->rendercmp[0].vao_, 0);
		glEnableVertexArrayAttrib(data->rendercmp[0].nvao_, 2);

		glVertexArrayAttribBinding(data->rendercmp[0].vao_, 0, 0);
		glVertexArrayAttribBinding(data->rendercmp[0].nvao_, 2, 0);
		// Back here when applying normals etc:
		glVertexArrayAttribFormat(data->rendercmp[0].vao_, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribFormat(data->rendercmp[0].nvao_, 2, 3, GL_FLOAT, GL_FALSE, 0);
		//

		glVertexArrayVertexBuffer(data->rendercmp[0].vao_, 0, data->rendercmp[0].vbo_, 0, 3 * sizeof(GLuint));
		glVertexArrayVertexBuffer(data->rendercmp[0].nvao_, 2, data->rendercmp[0].nbo_, 0, 3 * sizeof(GLuint));

		glVertexArrayElementBuffer(data->rendercmp[0].vao_, data->rendercmp[0].ebo_);
		glVertexArrayElementBuffer(data->rendercmp[0].nvao_, data->rendercmp[0].enbo_);


		if (geo.texCoords_.size() > 0) {

			if (data->rendercmp[0].tvao_ == 999) {
				GLuint texlocation = 1;
				glCreateVertexArrays(1, &data->rendercmp[0].tvao_);
				glCreateBuffers(1, &data->rendercmp[0].tbo_);
				glCreateBuffers(1, &data->rendercmp[0].etbo_);

				glNamedBufferData(data->rendercmp[0].tbo_, geo.texCoords_.size() * sizeof(float),
					geo.texCoords_.data(), GL_STATIC_DRAW);
				glNamedBufferData(data->rendercmp[0].etbo_, geo.texCoordsIndices_.size() * sizeof(uint32_t),
					geo.texCoordsIndices_.data(), GL_STATIC_DRAW);

				glEnableVertexArrayAttrib(data->rendercmp[0].tvao_, texlocation);
				glVertexArrayAttribBinding(data->rendercmp[0].tvao_, texlocation, 0);
				glVertexArrayAttribFormat(data->rendercmp[0].tvao_, texlocation, 2, GL_FLOAT, GL_FALSE, 0);

			glVertexArrayVertexBuffer(data->rendercmp[0].tvao_, texlocation, data->rendercmp[0].tbo_, 0, 2 * sizeof(GLuint));
			glVertexArrayElementBuffer(data->rendercmp[0].tvao_, data->rendercmp[0].etbo_);
		}
	}
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
		preExecDone_ = 1;
	}

	void Core::ExecSystems()
	{
		for (unsigned int i = 0; i < data->sys.size()-1; i++) {
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
