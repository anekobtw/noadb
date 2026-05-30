#ifndef TABLE_H
#define TABLE_H

#include "types.h"

Table *create_table(char *table_name);
void add_column(Table *table, ColumnType column_type, char *column_name);
void free_table(Table *table);

#endif
