#include "roadmap_parser.h"
#include "../Utils/haversine.h"
#include "../Models/transport.h"  
#include <stdlib.h>
#include <string.h>

bool load_roadmap(Graph* graph, const char* filename) {

    if (!graph || !filename) 
    
               return false;

    printf("Loading roadmap from %s...\n", filename);

    CSVData* data = parse_csv_file(filename);

    if (!data)
    
         return false;

    int segments_loaded = 0;

    for (int i = 0; i < data->num_rows; i++) {
        
        if (data->rows[i].num_fields < 3)
        
                   continue; 

        const char* transport_name = get_csv_field(&data->rows[i], 0);
        TransportMode mode = get_transport_mode_from_name(transport_name);

        if ((int)mode < 0)
        
                  mode = TRANSPORT_CAR;

        
        int total_fields = data->rows[i].num_fields;
        int prev_node_id = -1;

        
        for (int j = 1; j < total_fields - 1; j += 2) {

                     const char* lon_str = get_csv_field(&data->rows[i], j);
                     const char* lat_str = get_csv_field(&data->rows[i], j + 1);
            
            if (!lon_str || !lat_str || strlen(lon_str) == 0)
            
                        continue;

            double lon = atof(lon_str);
            double lat = atof(lat_str);

            
            Node* existing = find_node_by_location(graph, lat, lon, 0.0001);
            int node_id;

            if (existing) {

                   node_id = existing->id;
            } 
            
            else {

                     node_id = add_node(graph, lat, lon, "", NODE_ROAD);
            }

            if (prev_node_id != -1 && prev_node_id != node_id) {

                Node* prev_node = get_node(graph, prev_node_id);
                double dist = haversine_distance(prev_node->latitude, prev_node->longitude, lat, lon);

                
                add_bidirectional_edge(graph, prev_node_id, node_id, dist, mode);
            }
                prev_node_id = node_id;
        }
                segments_loaded++;
    }

    free_csv_data(data);
    printf("Successfully loaded %d road segments.\n", segments_loaded);
    return true;
}