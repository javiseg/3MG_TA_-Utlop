#ifndef UTLOP_MATERIAL
#define UTLOP_MATERIAL 1

#include "glad/glad.h"

namespace Utlop
{

  class Material
  {
    public:
      Material();
      ~Material();

      void init();
      void draw();
      virtual void setParameters();

      void loadShader(char* vertexFilename, char* fragmentFilename);
      void setBool(const char* name, bool value);
      void setInt(const char* name, int value);
      void setFloat(const char* name, float value);

    protected:

    private:
      GLuint _vertex_shader;
      GLuint _fragment_shader;
      GLuint _shader;

      void loadVertexShader(char* filename);
      void loadFragmentShader(char* filename);

  };


}

#endif // !UTLOP_MATERIAL
