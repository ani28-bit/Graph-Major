#include "route_parser.h"
#include "../Utils/haversine.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool load_route(Graph* graph, const char* filename, 
                        TransportMode mode, NodeType node_type) {

    if (!graph || !filename) 
    
             return false;

    printf("Loading routes from %s...\n", filename);

    CSVData* data = parse_csv_file(filename);

    if (!data) 
    
        return false;

    int routes_loaded = 0;

    for (int i = 0; i < data->num_rows; i++) {
        
        if (data->rows[i].num_fields < 5) 
        
                    continue;

        const char* start_name = get_csv_field(&data->rows[i], data->rows[i].num_fields - 2);
        const char* end_name   = get_csv_field(&data->rows[i], data->rows[i].num_fields - 1);

        int num_coord_fields = data->rows[i].num_fields - 3; 
        int num_coords = num_coord_fields / 2;

        int prev_node_id = -1;

        for (int j = 0; j < num_coords; j++) {
           
            double lon = atof(get_csv_field(&data->rows[i], 1 + (j * 2)));
            double lat = atof(get_csv_field(&data->rows[i], 2 + (j * 2)));

            const char* station_name = "";

            if (j == 0) 
            
                   station_name = start_name;

            else if (j == num_coords - 1) 
            
                   station_name = end_name;

            
                int node_id = add_node(graph, lat, lon, station_name, node_type);

            if (prev_node_id != -1) {

                Node* prev_node = get_node(graph, prev_node_id);
                double dist = haversine_distance(prev_node->latitude, prev_node->longitude, lat, lon);
                add_bidirectional_edge(graph, prev_node_id, node_id, dist, mode);
            }
                prev_node_id = node_id;
        } 
                routes_loaded++;
    }

                free_csv_data(data);
                return routes_loaded > 0;
 }



  bool load_metro_routes(Graph* graph, const char* filename) {

               return load_route(graph, filename, TRANSPORT_METRO, NODE_METRO_STATION);
  }

 bool load_bus_routes(Graph* graph, const char* filename, TransportMode bus_type) {

               return load_route(graph, filename, bus_type, NODE_BUS_STOP);
 }