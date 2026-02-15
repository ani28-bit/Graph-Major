#ifndef ROUTE_PARSER_H
#define ROUTE_PARSER_H

#include "../Graph/graph.h"
#include "csv_parser.h"

bool load_metro_routes(Graph* graph, const char* filename);

bool load_bus_routes(Graph* graph, const char* filename, TransportMode bus_type);

void connect_transport_to_roads(Graph* graph);

#endif 