#include "internal.h"
#include "table.h"
#include "types.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Database *init_db(char *filename) {
  // checking if it ends with .noa
  assert(strlen(filename) > 4);
  assert(strcmp(filename + (strlen(filename) - 4), ".noa") == 0);

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

  // insert header
  if (size < sizeof(int)) {
    _set_table_count(db, 0);
  }

  return db;
}

void close_db(Database *db) {
  if (!db)
    return;
  free(db->filename);
  free(db);
}

int get_table_count(Database *db) {
  FILE *file = fopen(db->filename, "r");
  assert(file);

  int table_count;
  size_t read = fread(&table_count, sizeof(int), 1, file);
  assert(read == 1);

  fclose(file);

  return table_count;
}

void add_table(Database *db, Table *table) {
  // increasing table count
  int tables = get_table_count(db);
  _set_table_count(db, ++tables);

  FILE *file = fopen(db->filename, "r+b");
  assert(file);

  fseek(file, 0, SEEK_END);

  // writing table name
  uint16_t table_name_len = strlen(table->table_name);
  fwrite(&table_name_len, sizeof(uint16_t), 1, file);
  fwrite(table->table_name, table_name_len, 1, file);

  // writing the amount of columns and rows (0 for rows)
  fwrite(&table->columns_len, sizeof(uint32_t), 1, file);
  uint32_t row_count = 0;
  fwrite(&row_count, sizeof(uint32_t), 1, file);

  // writing the columns
  for (uint32_t i = 0; i < table->columns_len; i++) {
    uint16_t column_name_len = strlen(table->columns[i].column_name);
    fwrite(&column_name_len, sizeof(uint16_t), 1, file);
    fwrite(table->columns[i].column_name, 1, column_name_len, file);
    fwrite(&table->columns[i].column_type, sizeof(ColumnType), 1, file);
  }

  fclose(file);
}

int main() {
  printf("Creating database..\n");

  Database *db = init_db("test.noa");

  if (db) {
    printf("Database created!\n");
    printf("Filename: %s\n", db->filename);
    printf("Table count: %d\n", get_table_count(db));
  } else {
    printf("Database couldn't be created :(");
    return 1;
  }

  Table *users_table = create_table("users");
  add_column(users_table, COLUMN_TYPE_UINT16, "id");
  add_column(users_table, COLUMN_TYPE_STRING, "name");
  add_column(users_table, COLUMN_TYPE_UINT8, "age");

  printf("Adding table %s into %s\n", users_table->table_name, db->filename);
  add_table(db, users_table);

  free_table(users_table);

  close_db(db);

  return 0;
}
