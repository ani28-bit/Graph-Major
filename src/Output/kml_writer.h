
#ifndef KML_WRITER_H
#define KML_WRITER_H

#include "../Models/route.h"
#include "../Graph/graph.h"


bool write_route_to_kml(Route* route, Graph* graph, 
                       const char* filename, int problem_number);


bool write_coordinates_to_kml(Location* coords, int num_coords,
                             const char* filename, const char* name);

void write_kml_header(FILE* fp);

void write_kml_footer(FILE* fp);

void write_kml_placemark(FILE* fp, Location* coords, int num_coords, 
                        const char* name, const char* description);

#endif 
