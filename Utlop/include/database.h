#pragma once
#include "data.h"

namespace Utlop {

  class DataBase {
    public:
      DataBase();
      ~DataBase();

      void LoadDatabase(const char* path, RenderCtx* data);
  };
}