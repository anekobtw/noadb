#ifndef DATABASE_H
#define DATABASE_H

#include <stddef.h>

typedef struct {
  char *filename;
} Database;

typedef enum {
  COLUMN_TYPE_INT8,
  COLUMN_TYPE_INT16,
  COLUMN_TYPE_INT32,
  COLUMN_TYPE_INT64,

  COLUMN_TYPE_UINT8,
  COLUMN_TYPE_UINT16,
  COLUMN_TYPE_UINT32,
  COLUMN_TYPE_UINT64,

  COLUMN_TYPE_CHAR,
  COLUMN_TYPE_STRING,

  COLUMN_TYPE_BOOL,
} ColumnType;

typedef struct {
  char *column_name;
  ColumnType column_type;
} Column;

typedef struct {
  char *table_name;
  size_t n;
  Column *columns;
} Table;

#endif
