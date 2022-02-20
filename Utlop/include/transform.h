#ifndef UTLOP_TRANSFORM
#define UTLOP_TRANSFORM 1

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace Utlop
{

  class Transform
  {
    public:
      Transform();
			~Transform();

      void setPosition(glm::vec3 position);
      void setScale(glm::vec3 scale);
      void setRotation(glm::vec3 rotation);

      void translate(glm::vec3 translation);
      void translate(glm::vec3 translation, float speed);
      void scale(glm::vec3 scale);
      void rotate(glm::vec3 rotation);
			void rotation_angle(float rot_ang);

			glm::vec3 getPosition();
			glm::vec3 getScale();
			glm::vec3 getRotation();
			float getRotationAngle();

			Transform& operator=(const Transform& other);
			Transform(const Transform& other);
			Transform(Transform&& other);
    protected:

    private:
      glm::vec3 position_;
      glm::vec3 scale_;
      glm::vec3 rotation_;
			float rotation_angle_;
  };


}

#endif // !UTLOP_TRANSFORM
