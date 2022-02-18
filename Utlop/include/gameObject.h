#ifndef UTLOP_GAMEOBJECT
#define UTLOP_GAMEOBJECT 1

#include "mesh.h"

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

      void setTransform(Transform transform);
			void setScale(glm::vec3 scale);
			void setRotation(glm::vec3 rotation);
			void setPosition(glm::vec3 position);
			void setColor(glm::vec3 color);
			void translate(glm::vec3 position, float speed);

			void setBasicGeometry(Utlop::Geo geometry);
			void setObjectGeometry(const char* src);


			GameObject& operator=(const GameObject& other);
			GameObject(const GameObject& other);
			GameObject(GameObject&& other);
			


			//copia igual stdmove contr

    protected:

    private:
      GLuint _vao;
      GLuint _vbo;

      Transform _transform;
      std::shared_ptr<Mesh> _mesh;
      float *_vertices;
  };


}

#endif // !UTLOP_GAMEOBJECT
