#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../src/Parsers/csv_parser.h"

void test_parse_simple_csv() {

    printf("Testing simple CSV parsing.\n");
    
   
    FILE* fp = fopen("/tmp/test_simple.csv", "w");
    fprintf(fp, "field1,field2,field3\n");
    fprintf(fp, "value1,value2,value3\n");
    fclose(fp);
    
    CSVData* data = parse_csv_file("/tmp/test_simple.csv");
    
    assert(data != NULL);
    assert(data->num_rows == 2);
    assert(data->rows[0].num_fields == 3);
    
    assert(strcmp(get_csv_field(&data->rows[0], 0), "field1") == 0);
    assert(strcmp(get_csv_field(&data->rows[0], 1), "field2") == 0);
    assert(strcmp(get_csv_field(&data->rows[1], 0), "value1") == 0);
    
    free_csv_data(data);
    remove("/tmp/test_simple.csv");
    
    printf("simple CSV parsing passed\n");
}

void test_parse_quoted_fields() {

            printf("Testing quoted fields...\n");
    
             FILE* fp = fopen("/tmp/test_quoted.csv", "w");
             fprintf(fp, "\"name\",\"address\",\"city\"\n");
             fprintf(fp, "\"John Doe\",\"123 Main St\",\"Dhaka\"\n");
              fclose(fp);
    
    CSVData* data = parse_csv_file("/tmp/test_quoted.csv");
    
    assert(data != NULL);
    assert(data->num_rows == 2);
    
    assert(strcmp(get_csv_field(&data->rows[1], 0), "John Doe") == 0);
    assert(strcmp(get_csv_field(&data->rows[1], 1), "123 Main St") == 0);
    
    free_csv_data(data);
    remove("/tmp/test_quoted.csv");
    
    printf("quoted fields passed\n");
}

 void test_parse_coordinate_list() {

         printf("Testing coordinate list parsing.\n");
    
         const char* coords = "90.363833,23.834145,90.364000,23.834200";
    
         int num_coords;
          Location* locations = parse_coordinate_list(coords, &num_coords);
    
          assert(locations != NULL);
          assert(num_coords == 2);
    
          assert(locations[0].longitude == 90.363833);
          assert(locations[0].latitude == 23.834145);
          assert(locations[1].longitude == 90.364000);
          assert(locations[1].latitude == 23.834200);
    
          free(locations);
    
          printf("coordinate list parsing passed\n");
 }

 void test_parse_single_coordinate() {

    printf("Testing single coordinate.\n");
    
    const char* coords = "90.5,23.8";
    
    int num_coords;
    Location* locations = parse_coordinate_list(coords, &num_coords);
    
    assert(locations != NULL);
    assert(num_coords == 1);
    assert(locations[0].longitude == 90.5);
    assert(locations[0].latitude == 23.8);
    
    free(locations);
    
    printf(" single coordinate passed\n");
}

void test_parse_empty_csv() {

    printf("Testing empty CSV.\n");
    
    FILE* fp = fopen("/tmp/test_empty.csv", "w");
    fclose(fp);
    
    CSVData* data = parse_csv_file("/tmp/test_empty.csv");
    
    assert(data != NULL);
    assert(data->num_rows == 0);
    
    free_csv_data(data);
    remove("/tmp/test_empty.csv");
    
    printf("empty CSV passed\n");
}

 void test_parse_whitespace() {

    printf("Testing whitespace handling.\n");
    
    FILE* fp = fopen("/tmp/test_whitespace.csv", "w");
    fprintf(fp, " field1 , field2 , field3 \n");
    fprintf(fp, " value1 , value2 , value3 \n");
    fclose(fp);
    
    CSVData* data = parse_csv_file("/tmp/test_whitespace.csv");
    
    assert(data != NULL);
    
   
    assert(strcmp(get_csv_field(&data->rows[0], 0), "field1") == 0);
    assert(strcmp(get_csv_field(&data->rows[1], 1), "value2") == 0);
    
    free_csv_data(data);
    remove("/tmp/test_whitespace.csv");
    
    printf("whitespace handling passed\n");
}

void test_parse_roadmap_format() {

      printf("Testing roadmap CSV format.\n");
    
       FILE* fp = fopen("/tmp/test_roadmap.csv", "w");
       fprintf(fp, "DhakaStreet,90.363833,23.834145,90.364000,23.834200,0,0.025\n");
       fclose(fp);
    
       CSVData* data = parse_csv_file("/tmp/test_roadmap.csv");
    
      assert(data != NULL);
      assert(data->num_rows == 1);
      assert(data->rows[0].num_fields == 7);
      
      assert(strcmp(get_csv_field(&data->rows[0], 0), "DhakaStreet") == 0);
    
   
      const char* coord_str = get_csv_field(&data->rows[0], 1);
   
      free_csv_data(data);
      remove("/tmp/test_roadmap.csv");
    
      printf("roadmap format passed\n");
 }

 void test_parse_route_format() {

      printf("Testing route CSV format.\n");
    
      FILE* fp = fopen("/tmp/test_route.csv", "w");
      fprintf(fp, "DhakaMetroRail,90.363833,23.834145,90.364600,23.834700,Uttara,Airport\n");
      fclose(fp);
    
      CSVData* data = parse_csv_file("/tmp/test_route.csv");
    
      assert(data != NULL);
      assert(data->num_rows == 1);
      assert(data->rows[0].num_fields == 7);
    
      assert(strcmp(get_csv_field(&data->rows[0], 0), "DhakaMetroRail") == 0);
      assert(strcmp(get_csv_field(&data->rows[0], 5), "Uttara") == 0);
      assert(strcmp(get_csv_field(&data->rows[0], 6), "Airport") == 0);
    
      free_csv_data(data);
      remove("/tmp/test_route.csv");
    
       printf("route format passed\n");
 } 

 void test_parse_multiple_coordinates() {

          printf("Testing multiple coordinates.\n");
    
          const char* coords = "90.1,23.1,90.2,23.2,90.3,23.3,90.4,23.4";
    
          int num_coords;
          Location* locations = parse_coordinate_list(coords, &num_coords);
    
          assert(locations != NULL);
          assert(num_coords == 4);
    
          assert(locations[0].longitude == 90.1);
          assert(locations[1].longitude == 90.2);
          assert(locations[2].longitude == 90.3);
          assert(locations[3].longitude == 90.4);
    
          free(locations);
    
           printf("multiple coordinates passed\n");
 }

 int main() {

       printf("Running Parser Tests\n\n");
    
          test_parse_simple_csv();
          test_parse_quoted_fields();
          test_parse_coordinate_list();
          test_parse_single_coordinate();
          test_parse_empty_csv();
          test_parse_whitespace();
          test_parse_roadmap_format();
          test_parse_route_format();
          test_parse_multiple_coordinates();
    
           printf("\nAll Parser Tests Passed!\n");
    
           return 0;
  }