#ifndef ROADMAP_PARSER_H
#define ROADMAP_PARSER_H

#include "../Graph/graph.h"
#include "csv_parser.h"

bool load_roadmap(Graph* graph, const char* filename);

void connect_transport_to_roads(Graph* graph);

#endif 