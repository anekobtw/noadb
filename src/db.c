#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

Database *init_db(char *filename) {
  // checking if it ends with .noa
  if (strlen(filename) <= 4)
    return NULL;
  if (strcmp(filename + (strlen(filename) - 4), ".noa"))
    return NULL;

  // create file if it doesn't exist
  FILE *file = fopen(filename, "r+b");
  if (!file) {
    file = fopen(filename, "wb");
  }

  // insert header if it doesn't exist
  struct stat file_stat;
  if (stat(filename, &file_stat) != 0)
    return NULL;

  if (file_stat.st_size < sizeof(int)) {
    fseek(file, 0, SEEK_SET);
    int tables_count = 0;
    fwrite(&tables_count, sizeof(int), 1, file);
  }

  fclose(file);

  // return a database object
  Database *db = malloc(sizeof(Database));
  if (!db)
    return NULL;

  db->filename = strdup(filename);
  if (!db->filename)
    return NULL;

  return db;
}

int free_db(Database *db) {
  if (!db)
    return 1;
  free(db->filename);
  free(db);
  return 0;
}

int add_table(Database *db, Table *table) {
  // increasing table count
  FILE *file = fopen(db->filename, "r+b");
  if (!file)
    return 1;

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

  // updating the header
  int table_count;
  fseek(file, 0, SEEK_SET);
  size_t read = fread(&table_count, sizeof(int), 1, file);
  if (!read)
    return -1;

  table_count++;
  fseek(file, 0, SEEK_SET);
  fwrite(&table_count, sizeof(int), 1, file);

  fclose(file);

  return 0;
}

Table *create_table(char *table_name) {
  size_t n = strlen(table_name);
  if (n > UINT16_MAX)
    return NULL;

  Table *table = malloc(sizeof(Table));
  if (!table)
    return NULL;

  table->table_name = strdup(table_name);
  if (!table->table_name)
    return NULL;

  table->columns_len = 0;
  table->columns = NULL;

  return table;
}

int add_column(Table *table, ColumnType column_type, char *column_name) {
  size_t n = strlen(column_name);
  if (n > UINT16_MAX)
    return 1;

  table->columns_len++;

  Column *temp = realloc(table->columns, table->columns_len * sizeof(Column));
  if (!temp)
    return 1;
  table->columns = temp;

  table->columns[table->columns_len - 1].column_name = strdup(column_name);
  if (!table->columns[table->columns_len - 1].column_name)
    return 1;
  table->columns[table->columns_len - 1].column_type = column_type;

  return 0;
}

int free_table(Table *table) {
  if (!table)
    return 1;

  for (uint32_t i = 0; i < table->columns_len; i++) {
    free(table->columns[i].column_name);
  }

  free(table->columns);
  free(table->table_name);
  free(table);

  return 0;
}
