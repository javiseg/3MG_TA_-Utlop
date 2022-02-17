#ifndef UTLOP_MESH
#define UTLOP_MESH 1

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <memory>
#include "material.h"
#include "geometry.h"

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

			void setMaterialParameters(const int uniform_pos, const Utlop::Type uniform_type, const float* value);
      void setMaterial(Material material);
			void setColor(glm::vec3 color);
			void setPosition(glm::vec3 position);
			void translate(glm::vec3 position, float speed);
			void createCube();
			void createTriangle();
			void createObject();

			Mesh& operator=(const Mesh& other);
			Mesh(std::shared_ptr<Mesh> other);
			Mesh(const Mesh& other);
			Mesh(Mesh&& other);

    protected:

    private:
      GLuint _vao;
      GLuint _vbo;
			GLuint _ebo;
			GLuint _texture;

      std::shared_ptr<Material> _material;
			int n_vertice_;
  };


}

#endif // !UTLOP_MESH
