#ifndef DB_H
#define DB_H

#include "types.h"

Database *init_db(char *filename);
void close_db(Database *db);

Table *create_table(char *table_name);
void add_column(Table *table, ColumnType column_type, char *column_name);
void free_table(Table *table);

void add_table(Database *db, Table *table);

#endif
