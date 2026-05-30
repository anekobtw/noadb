#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *filename;
} Database;

Database *init_db(char *filename) {
  // checking if it ends with .noa
  size_t filename_l = strlen(filename);
  size_t suffix_l = 4;

  int ends_with_noa = 0;

  if (filename_l > suffix_l) {
    ends_with_noa = strcmp(filename + (filename_l - suffix_l), ".noa") == 0;
  }

  assert(ends_with_noa);

  // creating a file
  FILE *file = fopen(filename, "w");
  assert(file);
  fclose(file);

  // creating a database object
  Database *db = malloc(sizeof(Database));
  assert(db);

  db->filename = strdup(filename);
  assert(db->filename);

  return db;
}

int main() {
  printf("Creating a file test.noa\n");

  Database *db = init_db("test.noa");

  if (db) {
    printf("Database created!\n");
    printf("Filename: %s\n", db->filename);
  }

  return 0;
}
