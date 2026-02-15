#include "csv_parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



static char* trim(char* str) {

    if (!str){
    
        return NULL;
    }

    while (*str && (*str == ' ' || *str == '\t' || *str == '\r' || *str == '\n'))
    
                 str++;

    if (*str == '\0'){
    
            return str;
    }

    char* end = str + strlen(str) - 1;

    while (end > str && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) 
               
             end--;

           *(end + 1) = '\0';
            return str;
 }

 static void parse_csv_line(char* line, CSVRow* row) {

    row->fields = NULL;
    row->num_fields = 0;

    if (!line || *line == '\0'){ 
    
            return;
    }

    int field_count = 1;
    bool in_quotes = false;

    for (char* p = line; *p; p++) {

        if (*p == '"'){ 
         
               in_quotes = !in_quotes;
        }

        else if (*p == ',' && !in_quotes) {
        
                   field_count++;
        }
    }

    row->fields = (char**)malloc(field_count * sizeof(char*));
    char* field_start = line;
    in_quotes = false;
    int field_idx = 0;

    for (char* p = line; ; p++) {

        if (*p == '"'){ 
        
               in_quotes = !in_quotes;
        }

        else if ((*p == ',' || *p == '\0') && !in_quotes) {

            char saved = *p;
            *p = '\0';
            
            char* field = field_start;

            if (*field == '"') {

                   field++;
                   int len = strlen(field);

                if (len > 0 && field[len-1] == '"') 
                
                      field[len-1] = '\0';
            }
            
                   row->fields[field_idx++] = strdup(trim(field));

                   if (saved == '\0') 
                   
                         break;
                       field_start = p + 1;
               }
          }
                       row->num_fields = field_idx;
  }

      CSVData* parse_csv_file(const char* filename) {

         FILE* fp = fopen(filename, "r");

         if (!fp) {
         
             return NULL;
         }

     CSVData* data = (CSVData*)malloc(sizeof(CSVData));
     data->rows = NULL;
     data->num_rows = 0;
    
    int capacity = 500; 
    data->rows = (CSVRow*)malloc(capacity * sizeof(CSVRow));

    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), fp)) {

        line[strcspn(line, "\r\n")] = '\0';

        if (strlen(line) == 0){ 
        
                 continue;
        }

        if (data->num_rows >= capacity) {

            capacity *= 2;
            CSVRow* new_rows = (CSVRow*)realloc(data->rows, capacity * sizeof(CSVRow));

            if (!new_rows){
            
                   break;
            }
                   data->rows = new_rows;
         }

                parse_csv_line(line, &data->rows[data->num_rows]);
                data->num_rows++;
     }
                 fclose(fp);
                 return data;
 }


 Location* parse_coordinate_list(const char* coord_string, int* num_coords) {

    if (!coord_string || !num_coords || strlen(coord_string) == 0) {

            if(num_coords){ 
            
                  *num_coords = 0;
            }
                   return NULL;
    }

      char* str = strdup(coord_string);
      char* token;
    
    
     int comma_count = 0;
     for (int i = 0; coord_string[i]; i++) 
     
           if (coord_string[i] == ','){
           
                   comma_count++;
           }
    
    int max_points = comma_count + 1;
    Location* locations = (Location*)malloc((max_points / 2 + 1) * sizeof(Location));
    int count = 0;

    token = strtok(str, ",");

    while (token != NULL) {

        double lat = atof(trim(token));

        token = strtok(NULL, ",");

        if (token != NULL) {

            double lon = atof(trim(token));
            locations[count].latitude = lat;
            locations[count].longitude = lon;
            count++;
            token = strtok(NULL, ",");
        }
         else {

                 break; 
        }
    }

       *num_coords = count;
       free(str);
       return locations;
}

 void free_csv_data(CSVData* data) {

    if (!data){
     
          return;
    }

    for (int i = 0; i < data->num_rows; i++) {

          for (int j = 0; j < data->rows[i].num_fields; j++) {

                    free(data->rows[i].fields[j]);
        }
                    free(data->rows[i].fields);
     }
                    free(data->rows);
                    free(data);
 }


 void print_csv_data(CSVData* data) {

    if (!data) {

          printf("CSV Data is NULL.\n");
          return;
    }

      printf("--- CSV Data Summary (%d rows) ---\n", data->num_rows);
    
    for (int i = 0; i < data->num_rows; i++) {

                printf("Row %-3d [%2d fields]: ", i, data->rows[i].num_fields);
        
        for (int j = 0; j < data->rows[i].num_fields; j++) {

            
            const char* field = data->rows[i].fields[j];

            printf("[%s]%s", 
                    (field && strlen(field) > 0) ? field : "EMPTY", 
                     (j == data->rows[i].num_fields - 1) ? "" : " | ");
        }
                      printf("\n");
    }
                       printf(" End of CSV Data\n");
 }

  const char* get_csv_field(CSVRow* row, int field_index) {

       if (!row || field_index < 0 || field_index >= row->num_fields) {

                return NULL;
    }
                return row->fields[field_index];
}


 char* get_csv_field_from_line(char* line, int field_index) {

    if (!line) {
    
          return NULL;
    }
    static char buffer[1024];
    int current_field = 0;
    int i = 0, j = 0;
    bool in_quotes = false;
    
    while (line[i] != '\0' && line[i] != '\n') {

        if (line[i] == '"') {

            in_quotes = !in_quotes;
            i++;
            continue;
        }
        
        if (line[i] == ',' && !in_quotes) {

            if (current_field == field_index) {

                buffer[j] = '\0';
                return buffer;
            }

            current_field++;
            j = 0;
            i++;
            continue;
        }
        
        if (current_field == field_index) {

                 buffer[j++] = line[i];
        }
                  i++;
    }
    
   
    if (current_field == field_index) {

            buffer[j] = '\0';
            return buffer;
    }
    
             return NULL;
} 