#ifndef UTLOP_GAMEOBJECT
#define UTLOP_GAMEOBJECT 1

#include "mesh.h"
#include "texture.h"
#include "transform.h"

namespace Utlop
{

  class GameObject
  {
    public:
      GameObject();
      ~GameObject();

      void init();
      void draw();
      void update();
      void destroy();

      void setTransform(Transform transform);
			void setScale(glm::vec3 scale);
			void setRotation(glm::vec3 rotation);
			void setPosition(glm::vec3 position);
			void setColor(glm::vec3 color);
			void translate(glm::vec3 position, float speed);

			void addMesh(int indexmesh);
			void setShader(char* vertex, char* fragment);
			void setTexture(char* path);

			GameObject& operator=(const GameObject& other);
			GameObject(const GameObject& other);
			GameObject(GameObject&& other);
			


			//copia igual stdmove contr

    protected:

    private:

      Transform transform_;
			vector<GLuint> meshIndices_;
			vector<Transform> transformVector_;
			vector<Texture> textures_;
			vector<TextureCoords> textureCoords_;
			std::shared_ptr<Shader> shader_;

			int projection_mat_index_;
  };


}

#endif // !UTLOP_GAMEOBJECT
