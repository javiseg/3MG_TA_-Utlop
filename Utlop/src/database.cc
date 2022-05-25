#include "database.h"
#include "sqlite3.h"
#include "component.h"
#include "core.h"


int getSavedSceneCallback(void* database, int number_cols, char** values, char** column) {
  sqlite3* db = (sqlite3*)database;
  
  // This function is in database_character.cc
  // Declared in database.h


  const enum DatabaseCamps {
    kID = 0,
    kID_Components = 1,
    kLocation_X = 2,
    kLocation_Y = 3,
    kLocation_Z = 4,
    kMesh = 5,
    kMaterial = 6,
    kParentIdx = 7
  };

  Utlop::RenderCtx* data = Utlop::Core::Instance()->getData();

  int newEntity = Utlop::Core::Instance()->AddEntity();

  data->entities[newEntity]->componentsID_ = strtol(values[kID_Components], NULL, 10);
  Utlop::Core::Instance()->AddAllComponents(*data->entities[newEntity], data->entities[newEntity]->componentsID_);

  glm::vec3 transform = glm::vec3(strtol(values[kLocation_X], NULL, 10), 
    strtol(values[kLocation_Y], NULL, 10), strtol(values[kLocation_Z], NULL, 10));
  
  data->localtrcmp[data->entities[newEntity]->cmp_indx_[Utlop::kLocalTRCompPos]].position = transform;
  data->rendercmp[data->entities[newEntity]->cmp_indx_[Utlop::kRenderCompPos]].mesh_idx.push_back(strtol(values[kMesh], NULL, 10));
  data->rendercmp[data->entities[newEntity]->cmp_indx_[Utlop::kRenderCompPos]].material_idx.push_back(strtol(values[kMaterial], NULL, 10));
  
  int parent = strtol(values[kParentIdx], NULL, 10);
  if (parent != -1) {
    data->heritagecmp[data->entities[newEntity]->cmp_indx_[Utlop::kHeritageCompPos]].parentID = parent;
  }
  return 0;
}


Utlop::DataBase::DataBase()
{
}

Utlop::DataBase::~DataBase()
{
}

void Utlop::DataBase::LoadDatabase(const char* path, RenderCtx* data)
{
  sqlite3* db_ = NULL;
  char* messaggeError;
  int rc = 0;
  rc = sqlite3_open(path, &db_);

  if (rc != SQLITE_OK) {
    return;
  }
  string sql_query_ = "SELECT * FROM SAVED_SCENE";


  sqlite3_exec(db_, sql_query_.c_str(), getSavedSceneCallback, db_, &messaggeError);
}
