#ifndef UTLOP_MATERIAL
#define UTLOP_MATERIAL 1

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "transform.h"

namespace Utlop
{
	enum Type {
		T_NONE = 0x0,
		T_FLOAT_1,
		T_FLOAT_2,
		T_FLOAT_3,
		T_FLOAT_4,

		T_DOUBLE_1,
		T_DOUBLE_2,
		T_DOUBLE_3,
		T_DOUBLE_4,

		T_INT_1,
		T_INT_2,
		T_INT_3,
		T_INT_4,

		T_UINT_1,
		T_UINT_2,
		T_UINT_3,
		T_UINT_4,

		T_BYTE_1,
		T_BYTE_2,
		T_BYTE_3,
		T_BYTE_4,

		T_UBYTE_1,
		T_UBYTE_2,
		T_UBYTE_3,
		T_UBYTE_4,

		T_SHORT_1,
		T_SHORT_2,
		T_SHORT_3,
		T_SHORT_4,

		T_USHORT_1,
		T_USHORT_2,
		T_USHORT_3,
		T_USHORT_4,

		T_MAT_4x4,
		T_MAT_3x3,
		T_MAT_2x2,

		T_SAMPLER_1D,
		T_SAMPLER_2D,
		T_SAMPLER_3D,

		// aliases
		T_FLOAT = T_FLOAT_1,
		T_DOUBLE = T_DOUBLE_1,
		T_INT = T_INT_1,
		T_UINT = T_UINT_1,
		T_BYTE = T_BYTE_1,
		T_UBYTE = T_UBYTE_1,
		T_SHORT = T_SHORT_1,
		T_USHORT = T_USHORT_1,

		//--------------------------
		T_MAX_DEFINED // max value of defined types
	};
  class Material
  {
    public:
      Material();
      ~Material();

      void init();
      void draw();
      virtual void setParameters(const int uniform_pos, const Utlop::Type uniform_type, const float* value);

      void loadShader(char* vertexFilename, char* fragmentFilename);
      void setBool(const char* name, bool value);
      void setInt(const char* name, int value);
      void setFloat(const char* name, float value);

			void setColor(glm::vec3 color);
			void setPosition(glm::vec3 position);
			void translate(glm::vec3 position, float speed = 1.0f);
			void setTransform(Utlop::Transform transform);
			

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

  };


}

#endif // !UTLOP_MATERIAL
