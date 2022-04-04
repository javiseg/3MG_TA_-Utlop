PROJ_DIR = path.getabsolute("./..")

solution("Utlop")
  
  location(PROJ_DIR .. "/build")
  language "C++"
  
  platforms {
  "native",
    "x32",
    "x64",
  }
  
  configurations {
    "Debug",
    "Relese"
  }
  
  project "Utlop"
  kind "StaticLib"
  UTLOP_DIR = path.join(PROJ_DIR, "Utlop")
  defines{"_GLFW_WIN32"}

  --Here is where we need to include the include directories (.h)
  includedirs{
	path.join(UTLOP_DIR, "./include/"),
	path.join(UTLOP_DIR, "./deps/glfw/include/"),
    path.join(UTLOP_DIR, "./deps/glad/include/"),
    path.join(UTLOP_DIR, "./deps/glm/include/"),
    path.join(UTLOP_DIR, "./deps/enkiTS/src/"),
    path.join(UTLOP_DIR, "./deps/math/"),
    path.join(UTLOP_DIR, "./deps/imgui/"),
    path.join(UTLOP_DIR, "./deps/imgui/backends/"),
    path.join(UTLOP_DIR, "./deps/sched/")
  }

  --Here only the files that we want compile.
  files{   
    path.join(UTLOP_DIR, "./**.cc"),
    path.join(UTLOP_DIR, "./**.h"),
    path.join(UTLOP_DIR, "./deps/glfw/src/context.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/init.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/input.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/monitor.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/vulkan.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/window.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/win32_init.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/win32_joystick.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/win32_monitor.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/win32_time.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/win32_thread.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/win32_window.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/wgl_context.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/egl_context.c"),
    path.join(UTLOP_DIR, "./deps/glfw/src/osmesa_context.c"),
    path.join(UTLOP_DIR, "./deps/glad/src/glad.c"),
    path.join(UTLOP_DIR, "./deps/enkiTS/src/TaskScheduler.cpp"),
    path.join(UTLOP_DIR, "./deps/math/"),
    path.join(UTLOP_DIR, "./deps/imgui/*.cc"),
    path.join(UTLOP_DIR, "./deps/imgui/*.cpp"),
    path.join(UTLOP_DIR, "./deps/imgui/backends/imgui_impl_glfw.cpp"),
    path.join(UTLOP_DIR, "./deps/imgui/backends/imgui_impl_opengl3.cpp"),

  }

    --If is needed include some .lib in links is the place that you need to put it.
    configuration "Release"
    links{"opengl32", "shell32", "user32", "gdi32"}
    defines{}
    flags{"optimize"}
    targetdir(path.join(UTLOP_DIR, "./bin/Release"))

    configuration "Debug"
    links{"opengl32", "shell32", "user32", "gdi32"}
    defines{"_DEBUG",
            "_CRT_SECURE_NO_WARNINGS",
    }
    flags{"Symbols",
          "ExtraWarnings",
          "Symbols",
          "NoPCH",
    }
    targetdir(path.join(UTLOP_DIR, "./bin/debug"))
	
  project "UtlopTests"
  kind "ConsoleApp"
  UTLOP_TEST_DIR = path.join(PROJ_DIR, "./UtlopTests")

  --Here is where we need to include the include directories (.h)
  includedirs{
	path.join(UTLOP_TEST_DIR, "./include/"),
	path.join(UTLOP_DIR, "./include/"),
	path.join(UTLOP_DIR, "./deps/glfw/include/"),
    path.join(UTLOP_DIR, "./deps/glad/include/"),
    path.join(UTLOP_DIR, "./deps/glm/include/"),
    path.join(UTLOP_DIR, "./deps/enkiTS/src/"),
    path.join(UTLOP_DIR, "./deps/math/"),
    path.join(UTLOP_DIR, "./deps/imgui/"),
    path.join(UTLOP_DIR, "./deps/imgui/backends/"),
    path.join(UTLOP_DIR, "./deps/sched/")
  }

  --Here only the files that we want compile.
  files{   
    path.join(UTLOP_TEST_DIR, "./**.cc"),
    path.join(UTLOP_TEST_DIR, "./**.h"),
    path.join(UTLOP_TEST_DIR, "./**.glsl"),
  }

    --If is needed include some .lib in links is the place that you need to put it.
    configuration "Release"
    --links{"opengl32", "shell32", "user32", "gdi32", path.join(UTLOP_DIR, "./bin/release/Utlop")}
    links{"opengl32", "shell32", "user32", "gdi32", "Utlop"}
	wholearchive{"Utlop"}
    defines{}
    flags{"optimize"}
    targetdir(path.join(UTLOP_TEST_DIR, "./bin/Release"))

    configuration "Debug"
    --links{"opengl32", "shell32", "user32", "gdi32", path.join(UTLOP_DIR, "./bin/debug/Utlop")}
    links{"opengl32", "shell32", "user32", "gdi32", "Utlop" }
	wholearchive{"Utlop"}
    defines{"_DEBUG",
            "_CRT_SECURE_NO_WARNINGS",
    }
    flags{"Symbols",
          "ExtraWarnings",
          "Symbols",
          "NoPCH",
    }
    targetdir(path.join(UTLOP_TEST_DIR, "./bin/debug"))
  
  