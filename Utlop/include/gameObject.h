#ifndef Utlop_GAMEOBJECT
#define Utlop_GAMEOBJECT 1

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "material.h"
#include "transform.h"

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

      void setMaterial(Material material);
      void setTransform(Transform transform);

    protected:

    private:
      GLuint _vao;
      GLuint _vbo;

      Material _material;
      Transform _transform;
      float *_vertices;
  };


}

#endif // !Utlop_GAMEOBJECT
