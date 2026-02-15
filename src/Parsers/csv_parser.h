#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include "../Utils/common.h"

 typedef struct CSVRow {

    char** fields;
    int num_fields;
} CSVRow;


 typedef struct CSVData {

    CSVRow* rows;
    int num_rows;
} CSVData;

 CSVData* parse_csv_file(const char* filename);


 const char* get_csv_field(CSVRow* row, int field_index);

 char* get_csv_field_from_line(char* line, int field_index);

 Location* parse_coordinate_list(const char* coord_string, int* num_coords);


 void free_csv_data(CSVData* data);


 void print_csv_data(CSVData* data);

 #endif 