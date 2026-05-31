#include "table.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
