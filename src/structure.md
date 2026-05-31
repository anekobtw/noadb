[ version ]
[ uint32_t tables_count ]

for table in tables_count:

    [ uint16_t table_name_length ]
    [ table_name ]
 
    [ uint32_t column_count ]
    [ uint32_t row_count ]

    for column in columns:
        [ uint16_t column_name_length ]
        [ column_name ]
        [ column_type ]

    for row in rows:
        for column in columns:
            [ value ]

