#include "kml_writer.h"
#include "../Models/transport.h"
#include <stdio.h>


const char* get_mode_color(TransportMode mode) {

    switch (mode) {

        case TRANSPORT_WALK:         return "ff00ff00"; 
        case TRANSPORT_CAR:          return "ff0000ff"; 
        case TRANSPORT_METRO:        return "ffff0000"; 
        case TRANSPORT_BUS_BIKOLPO:  return "ff00ffff"; 
        case TRANSPORT_BUS_UTTARA:   return "ffff00ff"; 
        default:                     return "ffffffff"; 
    }
}

void write_kml_header(FILE* fp) {

     if (!fp) 
        return;
    
    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fp, "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
    fprintf(fp, "<Document>\n");

    
    for (int i = 0; i <= TRANSPORT_BUS_UTTARA; i++) {

        fprintf(fp, "  <Style id=\"modeStyle%d\">\n", i);
        fprintf(fp, "    <LineStyle>\n");
        fprintf(fp, "      <color>%s</color>\n", get_mode_color((TransportMode)i));
        fprintf(fp, "      <width>4</width>\n");
        fprintf(fp, "    </LineStyle>\n");
        fprintf(fp, "  </Style>\n");
    }
}

 bool write_route_to_kml(Route* route, Graph* graph,
                          const char* filename, int problem_number) {

     if (!route || !graph || !filename) 
           return false;
    
     FILE* fp = fopen(filename, "w");

    if (!fp) {

          fprintf(stderr, "Error: Cannot create file %s\n", filename);
          return false;
    }
    
          write_kml_header(fp);
    
   
       RouteSegment* seg = route->head;
       int seg_idx = 1;
    
    while (seg) {

        TransportInfo info = get_transport_info(seg->mode);
        
        fprintf(fp, "  <Placemark>\n");
        fprintf(fp, "    <name>Prob %d - Seg %d: %s</name>\n", 
                     problem_number, seg_idx++, info.name);

        fprintf(fp, "    <styleUrl>#modeStyle%d</styleUrl>\n", seg->mode);
        fprintf(fp, "    <LineString>\n");
        fprintf(fp, "      <tessellate>1</tessellate>\n");
        fprintf(fp, "      <coordinates>\n");
        
        Node* from = get_node(graph, seg->from_node_id);
        Node* to = get_node(graph, seg->to_node_id);
        
        if (from)

             fprintf(fp, "        %.6f,%.6f,0\n", from->longitude, from->latitude);

        if (to)  

             fprintf(fp, "        %.6f,%.6f,0\n", to->longitude, to->latitude);
        
        fprintf(fp, "      </coordinates>\n");
        fprintf(fp, "    </LineString>\n");
        fprintf(fp, "  </Placemark>\n");
        
        seg = seg->next;
    }
    
      write_kml_footer(fp);
      fclose(fp);
    
      printf("KML file written: %s (Check in Google Earth/Maps)\n", filename);
      return true;
 }

 void write_kml_footer(FILE* fp) {

      if (!fp) 

         return;

    fprintf(fp, "</Document>\n");
    fprintf(fp, "</kml>\n");
}


  bool write_coordinates_to_kml(Location* coords, int num_coords,
                                 const char* filename, const char* name) {

    if (!coords || num_coords == 0 || !filename) 

                return false;
    
    FILE* fp = fopen(filename, "w");

     if (!fp) 

        return false;
    
    write_kml_header(fp);
    write_kml_placemark(fp, coords, num_coords, name, "Coordinate list");
    write_kml_footer(fp);
    
    fclose(fp);
    return true;
}

 void write_kml_placemark(FILE* fp, Location* coords, int num_coords,
                           const char* name, const char* description) {

       if (!fp || !coords) 
       
               return;
    
    fprintf(fp, "  <Placemark>\n");
    fprintf(fp, "    <name>%s</name>\n", name ? name : "Point");

       if (description && description[0]) {

                  fprintf(fp, "    <description>%s</description>\n", description);
    }

    fprintf(fp, "    <LineString>\n");
    fprintf(fp, "      <tessellate>1</tessellate>\n");
    fprintf(fp, "      <coordinates>\n");
    
    for (int i = 0; i < num_coords; i++) {

              fprintf(fp, "        %.6f,%.6f,0\n", coords[i].longitude, coords[i].latitude);
    }
    
    fprintf(fp, "      </coordinates>\n");
    fprintf(fp, "    </LineString>\n");
    fprintf(fp, "  </Placemark>\n");
}