#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task1.h"
#include "task2.h"

int main(int argc, char **argv)
{
    FILE *in = NULL;
    int nr_nodes = 0;
    int nr_edges = 0;
    int i;

    // DEBUG MODE
    argc = 2;
    argv[1] = "1";

    if (argc == 1)
    {
        printf("Select task 1 or 2!\n");
        exit(1);
    }

    if (strcmp(argv[1], "1") == 0)
    {
        char **names1;
        char **names2;
        int *costs;
        Graph graph;

        in = fopen("tema3.in", "rt");
        if (in == NULL)
        {
            printf("Error when reading from input file!\n");
            exit(1);
        }
        else
        {
            fscanf(in, "%d", &nr_nodes);
            fscanf(in, "%d", &nr_edges);
            fgetc(in);
            names1 = (char **)malloc(nr_edges * sizeof(char *));
            for (i = 0; i < nr_edges; ++i)
            {
                names1[i] = (char *)malloc(20 * sizeof(char));
            }
            names2 = (char **)malloc(nr_edges * sizeof(char *));
            for (i = 0; i < nr_edges; ++i)
            {
                names2[i] = (char *)malloc(20 * sizeof(char));
            }
            costs = (int *)malloc(nr_edges * sizeof(int));

            read_edges(&in, names1, names2, costs, nr_edges);

            graph.lists = (List *)malloc(nr_nodes * sizeof(List));
            graph = init_graph(graph, nr_nodes, nr_edges);
            graph = set_node_names(graph, names1, names2);
            //graph = build_graph(graph, names1, names2, costs);
        }
        fclose(in);
    }
    else if (strcmp(argv[1], "2") == 0)
    {
        printf("Task 2\n");
    }
    else
    {
        printf("Invalid task number, enter 1 or 2!\n");
        exit(1);
    }

    return 0;
}