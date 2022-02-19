#include "matrixThreadScene.h"
#include "core.h"

void MatrixThreadScene::start()
{
  matrix_a = matrix_b = matrix_c = nullptr;
  //init_matrix();
  //sum_matrix();
  //Utlop::Core::Instance()->getScheduler()->AddTaskSetToPipe(&task);
  //Utlop::Core::Instance()->getScheduler()->WaitforTask(&task);

	Utlop::GameObject object;

	object.init();
	object.setGeometry(Utlop::Geo::kConst_Cube);
	//object.setGeometry("../UtlopTests/src/obj/doc.obj");
	object.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	
	//triangle.setColor(color);
	gameObjects_.push_back(object);

}

void MatrixThreadScene::update()
{
  //_gameObjects.push_back(triangle);
}

void MatrixThreadScene::init_matrix()
{
  if (matrix_a)
  {
    free(matrix_a);
  }
  if (matrix_b)
  {
    free(matrix_b);
  }

  matrix_a = (char*)malloc(matrix_h * matrix_w);
  matrix_b = (char*)malloc(matrix_h * matrix_w);
  
  if (matrix_a && matrix_b)
  {
    for (int i = 0; i < matrix_h; i++)
    {
      int row = i * matrix_w;
      for (int j = 0; j < matrix_w; j++)
      {
        matrix_a[row + j] = (int)(rand() % 11);
        matrix_b[row + j] = (int)(rand() % 11);
        //printf("A: %d\n", matrix_a[row + j]);
        //printf("B: %d\n", matrix_b[row + j]);
      }
    }
    printf("Matrix completed\n");
  }
}

void MatrixThreadScene::sum_matrix()
{
  matrix_c = (char*)malloc(matrix_h * matrix_w);

  if (matrix_a && matrix_b && matrix_c)
  {
    for (int i = 0; i < matrix_h; i++)
    {
      int row = i * matrix_w;
      for (int j = 0; j < matrix_w; j++)
      {
        matrix_c[row + j] = matrix_a[row + j] + matrix_b[row + j];
      }
    }
    printf("Matrix C completed\n");
  }
}
