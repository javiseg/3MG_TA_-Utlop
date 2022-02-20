#ifndef UTLOPmesh_
#define UTLOPmesh_ 1


#include <memory>
#include "material.h"
#include "geometry.h"
#include "shader.h"
#include <vector>

namespace Utlop
{
  class Mesh
  {
    public:
      Mesh();
      ~Mesh();

      void init(Geo type);
      void init(Geo type, char* src);
      void draw(Shader& shader);
      void start();
      void update();
      void destroy();

			void setupMesh();

			void createObject(std::vector<float> vertices, std::vector<unsigned int> indices);

			Mesh& operator=(const Mesh& other);
			Mesh(std::shared_ptr<Mesh> other);
			Mesh(const Mesh& other);
			Mesh(Mesh&& other);

    protected:

    private:
      GLuint vao_;
      GLuint vbo_;
			GLuint ebo_;
			GLuint texture_;

      //std::shared_ptr<Material> material_;
			unsigned int id_geometry_;
			unsigned int id_texture_;
			Geo type_;
	};


}

#endif // !UTLOPmesh_
