#ifndef UTLOP_TRANSFORM
#define UTLOP_TRANSFORM 1

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace Utlop
{

  class Transform
  {
    public:
      Transform(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, 
                glm::vec3 scale = { 0.0f, 0.0f, 0.0f }, 
                glm::vec4 rotation = { 0.0f, 0.0f, 0.0f, 0.0f });
      ~Transform();

      void setPosition(glm::vec3 position);
      void setScale(glm::vec3 scale);
      void setRotation(glm::vec4 rotation);

      void translate(glm::vec3 translation);
      void translate(glm::vec3 translation, float speed);
      void scale(glm::vec3 scale);
      void rotate(glm::vec4 rotation);

    protected:

    private:
      glm::vec3 _position;
      glm::vec3 _scale;
      glm::vec4 _rotation;
  };


}

#endif // !UTLOP_TRANSFORM
