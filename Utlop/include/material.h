#ifndef Utlop_MATERIAL
#define Utlop_MATERIAL 1

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

      void loadShader(char* vertexFilename, char* fragmentFilename);

    protected:

    private:
      GLuint _vertex_shader;
      GLuint _fragment_shader;
      GLuint _shader;

      void loadVertexShader(char* filename);
      void loadFragmentShader(char* filename);

  };


}

#endif // !Utlop_MATERIAL
