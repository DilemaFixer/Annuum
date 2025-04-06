#include "b.h"

#define OBJ_DIR "obj"
#define PROG_NAME "anum"
#define GCC "gcc"
#define FLAGS "-Wall -Wextra -g -O2"

int main(void) {

  INFO("Start building Annuum\n");

  if (!dir_exists(OBJ_DIR))
    make_dir(OBJ_DIR, 0755);

  Array *c_filse = find_all_files("./src", "c");

  char *current_dir = pwd();
  char *obj_dir = pathjoin(current_dir, OBJ_DIR);

  Array *o_files = array_new(c_filse->count);
  for (size_t i = 0; i < c_filse->count; i++) {
    char* name = path_basename((char *)c_filse->items[i]);
    char* obj_file_path = pathjoin(obj_dir , name);
    obj_file_path = change_extension(obj_file_path , "o");
    
    RUN(GCC , "-c" , (char *)c_filse->items[i] , "-o" , obj_file_path , FLAGS);

    array_add(o_files , obj_file_path);
  }

  char *objs = o_files->items[0];

  for(size_t i = 1 ; i < o_files->count; i++){
    objs = strcat_new(objs , " ");
    objs = strcat_new(objs , o_files->items[i]); 
  }

  RUN(GCC , objs , "-o" , PROG_NAME);
  return 0;
}
