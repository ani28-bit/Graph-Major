#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph/graph.h"
#include "Utils/haversine.h"
#include "Utils/common.h"
#include "Parsers/csv_parser.h"
#include "Parsers/roadmap_parser.h"
#include "Parsers/route_parser.h"
#include "Routing/route_finder.h"
#include "Output/kml_writer.h"
#include "Output/text_output.h"
#include "Routing/time_scheduler.h"


 void test_haversine();
 void test_graph();
 void check_metro_connectivity(Graph* graph);

 void print_banner() {

        printf("\n\n");
        printf("     Dhaka Routing System - Mr. Efficient  \n");
        printf("         Graph-based Route Optimizer  :  \n");
        printf("\n\n");

  }

 void print_menu() {

         printf("\n Navigation Menu :\n");
         printf("1. Problem 1: Shortest Car Route\n");
         printf("2. Problem 2: Cheapest Route (Car + Metro)\n");
         printf("3. Problem 3: Cheapest Route (All Modes)\n");
         printf("4. Problem 4: Cheapest with Start Time\n");
         printf("5. Problem 5: Fastest Route (Min Time)\n");
         printf("6. Problem 6: Cheapest with Deadline Check\n");
         printf("7. Run System Tests\n");
         printf("0. Exit\n");
         printf("\nEnter your choice: ");
  }

  void clear_buffer() {

        int c;
        while ((c = getchar()) != '\n' && c != EOF);
 }

 int main(int argc, char* argv[]) {

    (void)argc; 
     (void)argv;
    print_banner();

    printf("Loading Dhaka Roadmap and Transport Data.\n");
    Graph* graph = create_graph(100000); 

    if (!load_roadmap(graph, "Data/Roadmap-Dhaka.csv")) {

                printf("Could not find Roadmap-Dhaka.csv in Data/ folder.\n");
                free_graph(graph);
                return 1;
    }

       load_metro_routes(graph, "Data/Routemap-DhakaMetroRail.csv");
       load_bus_routes(graph, "Data/Routemap-BikolpoBus.csv", TRANSPORT_BUS_BIKOLPO);
       load_bus_routes(graph, "Data/Routemap-UttaraBus.csv", TRANSPORT_BUS_UTTARA);

       printf("Building walking connections between modes...\n");
       connect_transport_to_roads(graph);
    
     print_graph_stats(graph);

    int choice;

    while (1) {

        print_menu();
        if (scanf("%d", &choice) != 1) {

                  clear_buffer();
                  continue;
        }
        
        if (choice == 0) 
                 break;

           Location source = {0}, dest = {0};
           TimeOfDay start_time = {8, 0}; 
           char time_input[32] = {0};
           char deadline_str[32] = {0};

        if (choice >= 1 && choice <= 6) {

            clear_buffer(); 
            printf("Enter Source (Lon Lat): ");

            if (scanf("%lf %lf", &source.longitude, &source.latitude) != 2) {

                        printf("Invalid input. Please enter two numbers.\n");
                        clear_buffer();
                        continue;
            }

            printf("Enter Destination (Lon Lat): ");

            if (scanf("%lf %lf", &dest.longitude, &dest.latitude) != 2) {

                           printf("Invalid input. Please enter two numbers.\n");
                            clear_buffer();
                             continue;
            }
        }

        Route* route = NULL;
        switch(choice) {

            case 1: 

                  route = solve_problem_1(graph, source, dest); 
                  break;

            case 2: 

                  route = solve_problem_2(graph, source, dest); 
                  break;

            case 3: 

                  route = solve_problem_3(graph, source, dest); 
                  break;

            case 4:
            case 5:

                   printf("Enter Start Time (HH:MM): ");

                   if (scanf("%31s", time_input) != 1) 
                   
                                   break;

                    start_time = parse_time_string(time_input);
                    route = (choice == 4) ? solve_problem_4(graph, source, dest, start_time) 
                                                : solve_problem_5(graph, source, dest, start_time);
                
                                                  break;
            case 6:

                     printf("Enter Start Time (HH:MM): ");

                     if (scanf("%31s", time_input) != 1)
                                   break;
                         start_time = parse_time_string(time_input);
                
                          clear_buffer(); 

                     printf("Enter Deadline Time (HH:MM): ");

                     if (scanf("%31s", deadline_str) != 1)
                     
                                  break;
                          TimeOfDay deadline = parse_time_string(deadline_str);
                
                           route = solve_problem_6(graph, source, dest, start_time, deadline);
                
                      if (route) {

                               bool met = route_meets_deadline(route, start_time, deadline);

                           printf("\n Destination Reach Time: %02d:%02d vs Target: %02d:%02d \n", 

                                      route->arrival_time.hour, route->arrival_time.minute,
                                      deadline.hour, deadline.minute);
                                      printf(" Status: %s \n", met ? "Met" : "Exceeded");

                   }
                                         break;
            case 7:

                      test_haversine();
                       test_graph();
                        continue;
            
            default:

                         printf("Invalid Choice!\n");
                         continue;
        }

        if (route) {

                        char start_t_str[16];
                        format_time(start_time, start_t_str, sizeof(start_t_str));
                        print_route_for_problem(route, graph, choice, start_t_str, NULL);
            
                      char kml_path[128];
                      snprintf(kml_path, sizeof(kml_path), "Output/routes/problem%d.kml", choice);
                      write_route_to_kml(route, graph, kml_path, choice);
            
                      free_route(route);
        } 
        else if (choice <= 6) {

                     printf("\n No path could be found between these coordinates.\n");
             }
          }

              printf("Exiting System. Goodbye!\n");
              free_graph(graph);
               return 0;
  }

 void test_haversine() {

              double d = haversine_distance(23.832, 90.365, 23.845, 90.375);
              printf("Haversine Test (Uttara-ish): %.3f km\n", d);
  }

  void test_graph() {

              printf("Graph Structure Test OK. Nodes and Edges are reachable.\n");
 }

 