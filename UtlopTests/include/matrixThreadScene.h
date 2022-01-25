#include "gameScene.h"

#include "TaskScheduler.h"

class MatrixThreadScene: public Utlop::GameScene
{
  public:
    void start() override;
    void update() override;
    void init_matrix();
    void sum_matrix();

  private:
    const int matrix_w = 8000;
    const int matrix_h = 8000;

    char* matrix_a;
    char* matrix_b;
    char* matrix_c;

    struct ParallelTaskSet : enki::ITaskSet {
      void ExecuteRange(enki::TaskSetPartition range_, uint32_t threadnum_) override {
        // do something here, can issue tasks with g_TS
        printf("Task");
      }
    };

    ParallelTaskSet task;

    Utlop::GameObject triangle;
    Utlop::GameObject triangle2;
};