#include "internal.h"
#include <assert.h>
#include <stdio.h>

void _set_table_count(Database *db, int new_tables_count) {
  FILE *file = fopen(db->filename, "r+b");
  assert(file);

  fseek(file, 0, SEEK_SET);
  fwrite(&new_tables_count, sizeof(int), 1, file);

  fclose(file);
}
