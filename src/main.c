#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *filename;
} Database;

void _manage_table_count(Database *db, int new_tables_count) {
  FILE *file = fopen(db->filename, "r+b");
  assert(file);

  fseek(file, 0, SEEK_SET);
  fwrite(&new_tables_count, sizeof(int), 1, file);

  fclose(file);
}

int get_table_count(Database *db) {
  FILE *file = fopen(db->filename, "rb");
  assert(file);

  int table_count;
  size_t read = fread(&table_count, sizeof(int), 1, file);
  assert(read == 1);

  fclose(file);

  return table_count;
}

Database *init_db(char *filename) {
  // checking if it ends with .noa
  size_t filename_l = strlen(filename);
  size_t suffix_l = 4;

  int ends_with_noa = 0;

  if (filename_l > suffix_l) {
    ends_with_noa = strcmp(filename + (filename_l - suffix_l), ".noa") == 0;
  }

  assert(ends_with_noa);

  // create file if it doesn't exist
  FILE *file = fopen(filename, "ab+");

  assert(file);

  fseek(file, 0, SEEK_END);
  long size = ftell(file);

  fclose(file);

  // return a database object
  Database *db = malloc(sizeof(Database));
  assert(db);

  db->filename = strdup(filename);
  assert(db->filename);

  if (size < sizeof(int)) {
    _manage_table_count(db, 0);
  }

  return db;
}

void close_db(Database *db) {
  if (!db) return;
  free(db->filename);
  free(db);
}

int main() {
  printf("Creating database..\n");

  Database *db = init_db("test.noa");

  if (db) {
    printf("Database created!\n");
    printf("Filename: %s\n", db->filename);
  } else {
    printf("Database couldn't be created :(");
    return 1;
  }

  printf("%d\n", get_table_count(db));

  _manage_table_count(db, 1);

  printf("%d\n", get_table_count(db));

  close_db(db);

  return 0;
}
