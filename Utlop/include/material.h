#ifndef UTLOP_MATERIAL
#define UTLOP_MATERIAL 1

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "glm/glm.hpp"
#include "transform.h"

namespace Utlop
{
	
  /*class Material
  {
    public:
      Material();
      ~Material();

      void init();
      void draw(int elements);
      virtual void setParameters(const int uniform_pos, const Utlop::Type uniform_type, const float* value);

      void loadShader(char* vertexFilename, char* fragmentFilename);
      void setBool(const char* name, bool value);
      void setInt(const char* name, int value);
      void setFloat(const char* name, float value);

			void setColor(glm::vec3 color);
			void setPosition(glm::vec3 position);
			void translate(glm::vec3 position, float speed = 1.0f);
			void setTransform(Utlop::Transform transform);
			
			void update();

			Material& operator=(const Material& other);

			Material(const Material& other);
			Material(Material&& other);



    protected:

    private:
      GLuint _vertex_shader;
      GLuint _fragment_shader;
      GLuint _shader;
			GLuint projection_mat_index;
			Transform _transform;

      void loadVertexShader(char* filename);
      void loadFragmentShader(char* filename);

  };*/


}

#endif // !UTLOP_MATERIAL
