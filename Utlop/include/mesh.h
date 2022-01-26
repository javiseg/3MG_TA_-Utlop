#ifndef UTLOP_MESH
#define UTLOP_MESH 1

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "material.h"

namespace Utlop
{

  class Mesh
  {
    public:
      Mesh();
      ~Mesh();

      void init();
      void draw();
      void start();
      void update();
      void destroy();

      void setMaterial(Material material);
			void setColor(float* color);

    protected:

    private:
      GLuint _vao;
      GLuint _vbo;

      Material _material;
      float *_vertices;
  };


}

#endif // !UTLOP_MESH
