#include "db.h"
#include "internal.h"
#include <assert.h>
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
  FILE *file = fopen(db->filename, "rb");
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

Table *create_table(char *table_name) {
  size_t n = strlen(table_name);
  assert(n <= UINT16_MAX);

  Table *table = malloc(sizeof(Table));
  assert(table);

  table->table_name = strdup(table_name);
  assert(table->table_name);

  table->columns_len = 0;
  table->columns = NULL;

  return table;
}

void add_column(Table *table, ColumnType column_type, char *column_name) {
  size_t n = strlen(column_name);
  assert(n <= UINT16_MAX);

  table->columns_len++;

  Column *temp = realloc(table->columns, table->columns_len * sizeof(Column));
  assert(temp);
  table->columns = temp;

  table->columns[table->columns_len - 1].column_name = strdup(column_name);
  assert(table->columns[table->columns_len - 1].column_name);
  table->columns[table->columns_len - 1].column_type = column_type;
}

void free_table(Table *table) {
  if (!table)
    return;

  for (uint32_t i = 0; i < table->columns_len; i++) {
    free(table->columns[i].column_name);
  }

  free(table->columns);
  free(table->table_name);
  free(table);
}
