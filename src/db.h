#ifndef DB_H
#define DB_H

#include "types.h"

Database *init_db(char *filename);
int free_db(Database *db);

Table *create_table(char *table_name);
int add_column(Table *table, ColumnType column_type, char *column_name);
int free_table(Table *table);

int add_table(Database *db, Table *table);

#endif
