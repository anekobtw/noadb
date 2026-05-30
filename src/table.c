#include "table.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

Table *create_table(char *table_name) {
  Table *table = malloc(sizeof(Table));
  assert(table);

  table->table_name = strdup(table_name);
  assert(table->table_name);

  table->n = 0;
  table->columns = NULL;

  return table;
}

void add_column(Table *table, ColumnType column_type, char *column_name) {
  table->n++;

  Column *temp = realloc(table->columns, table->n * sizeof(Column));
  assert(temp);
  table->columns = temp;

  table->columns[table->n - 1].column_name = strdup(column_name);
  assert(table->columns[table->n - 1].column_name);
  table->columns[table->n - 1].column_type = column_type;
}

void free_table(Table *table) {
  if (!table) return;

  for (size_t i = 0; i < table->n; i++) {
    free(table->columns[i].column_name);
  }

  free(table->columns);
  free(table->table_name);
  free(table);
}

