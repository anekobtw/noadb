#include "db.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *column_type_to_string(ColumnType type) {
  switch (type) {
  case COLUMN_TYPE_UINT8:
    return "UINT8";
  case COLUMN_TYPE_UINT16:
    return "UINT16";
  case COLUMN_TYPE_UINT32:
    return "UINT32";
  case COLUMN_TYPE_INT8:
    return "INT8";
  case COLUMN_TYPE_INT16:
    return "INT16";
  case COLUMN_TYPE_INT32:
    return "INT32";
  case COLUMN_TYPE_STRING:
    return "STRING";
  default:
    return "UNKNOWN";
  }
}

void print_schema(Database *db) {
  FILE *file = fopen(db->filename, "rb");
  if (!file)
    return;

  int table_count;
  size_t read = fread(&table_count, sizeof(int), 1, file);
  if (!read)
    return;

  printf("Database info:\n");
  printf("Filename: %s\n", db->filename);
  printf("Table count: %d\n", table_count);

  for (uint32_t i = 0; i < table_count; i++) {
    uint16_t table_name_len;
    fread(&table_name_len, sizeof(uint16_t), 1, file);
    char *table_name = malloc(table_name_len + 1);
    fread(table_name, table_name_len, 1, file);
    table_name[table_name_len] = '\0';
    printf("Table name: %s\n", table_name);

    uint32_t columns_len;
    fread(&columns_len, sizeof(uint32_t), 1, file);

    uint32_t row_count;
    fread(&row_count, sizeof(uint32_t), 1, file);

    for (uint32_t j = 0; j < columns_len; j++) {
      uint16_t column_name_len;
      fread(&column_name_len, sizeof(uint16_t), 1, file);
      char *column_name = malloc(column_name_len + 1);
      column_name[column_name_len] = '\0';
      fread(column_name, column_name_len, 1, file);
      ColumnType column_type;
      fread(&column_type, sizeof(ColumnType), 1, file);
      printf("%s %s", column_type_to_string(column_type), column_name);

      if (j != columns_len - 1) {
        printf(" | ");
      }
    }

    printf("\n");
  }

  fclose(file);
}

int main() {
  printf("DEBUG: Creating a database..\n");

  Database *db = init_db("test.noa");
  if (db)
    printf("DEBUG: Database created successfully!\n");

  Table *users_table = create_table("users");
  if (users_table) {
    if (add_column(users_table, COLUMN_TYPE_UINT16, "user_id") != 0)
      printf("Error adding column id to users_table\n");
    if (add_column(users_table, COLUMN_TYPE_STRING, "name") != 0)
      printf("Error adding column name to users_table\n");
    if (add_column(users_table, COLUMN_TYPE_UINT8, "age") != 0)
      printf("Error adding column age to users_table\n");
  }

  Table *posts_table = create_table("posts");
  if (posts_table) {
    if (add_column(posts_table, COLUMN_TYPE_UINT32, "id") != 0)
      printf("Error adding column id to users_table\n");
    if (add_column(posts_table, COLUMN_TYPE_UINT16, "user_id") != 0)
      printf("Error adding column user_id to users_table\n");
    if (add_column(posts_table, COLUMN_TYPE_STRING, "title") != 0)
      printf("Error adding column title to users_table\n");
  }

  printf("DEBUG: Adding table %s into %s\n", users_table->table_name,
         db->filename);
  if (add_table(db, users_table) != 0)
    printf("Error adding table %s into %s\n", users_table->table_name,
           db->filename);

  printf("DEBUG: Adding table %s into %s\n", posts_table->table_name,
         db->filename);
  if (add_table(db, posts_table) != 0)
    printf("Error adding table %s into %s\n", posts_table->table_name,
           db->filename);

  free_table(users_table);
  free_table(posts_table);

  printf("\n");
  print_schema(db);

  free_db(db);

  return 0;
}
