#ifndef UTLOP_GAMEOBJECT
#define UTLOP_GAMEOBJECT 1

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "material.h"
#include "transform.h"
#include "mesh.h"

namespace Utlop
{

  class GameObject
  {
    public:
      GameObject();
      ~GameObject();

      void init();
      void draw();
      void start();
      void update();
      void destroy();

      void setTransform(Transform transform);
			void setColor(float* color);


    protected:

    private:
      GLuint _vao;
      GLuint _vbo;

      Transform _transform;
      Mesh _mesh;
      float *_vertices;
  };


}

#endif // !UTLOP_GAMEOBJECT
