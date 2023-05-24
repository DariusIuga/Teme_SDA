#include "task1.h"
#include "task2.h"

void free_graph(Graph my_graph);
void free_digraph(Digraph my_graph);

int main(int argc, char **argv)
{
    FILE *in = NULL, *out = NULL;
    int nr_nodes = 0;
    int nr_edges = 0;
    int i;

    if (argc == 1)
    {
        printf("Select task 1 or 2!\n");
        exit(1);
    }

    in = fopen("tema3.in", "rt");
    if (in == NULL)
    {
        fprintf(stderr, "Error when reading from input file!\n");
        exit(1);
    }
    out = fopen("tema3.out", "wt");
    if (out == NULL)
    {
        fprintf(stderr, "Error when writing to output file!\n");
        exit(1);
    }

    if (strcmp(argv[1], "1") == 0)
    {
        int *min_costs;
        Graph graph;

        if (fscanf(in, "%d", &nr_nodes) != 1)
        {
            fprintf(stderr, "Error when reading nr of nodes!\n");
            exit(1);
        }
        if (fscanf(in, "%d", &nr_edges) != 1)
        {
            fprintf(stderr, "Error when reading nr of edges!\n");
            exit(1);
        }
        char(*names1)[MAX_STRLEN] = (char(*)[MAX_STRLEN])
            malloc(nr_edges * sizeof(char[MAX_STRLEN]));
        char(*names2)[MAX_STRLEN] = (char(*)[MAX_STRLEN])
            malloc(nr_edges * sizeof(char[MAX_STRLEN]));
        int *costs = (int *)malloc(nr_edges * sizeof(int));

        read_edges(&in, names1, names2, costs, nr_edges);

        // Initializarea grafului si alocarea memoriei pt vectorul de liste
        graph = init_graph(&graph, nr_nodes, nr_edges);
        // Asocierea intre numele nodurilor si listele de adiacenta
        graph = set_node_names(graph, names1, names2);
        // Adaugarea muchilor in liste
        graph = build_graph(graph, names1, names2, costs);

        free(names1);
        names1 = NULL;
        free(names2);
        names2 = NULL;
        free(costs);
        costs = NULL;

        int nr_components = count_connected_components(graph);
        // components este un vector al componentelor conexe din graf
        Graph *components = find_connected_components(graph, nr_components);
        // Vector cu suma costurilor din MST pentru fiecare componenta
        min_costs = (int *)malloc(nr_components * sizeof(int));
        for (i = 0; i < nr_components; ++i)
        {
            min_costs[i] = calculate_mst_cost(components[i]);
        }
        // Costurile sunt sortate in ordine crescatoare
        qsort(min_costs, nr_components, sizeof(int), cmp_int);

        fprintf(out, "%d\n", nr_components);
        for (i = 0; i < nr_components; ++i)
        {
            fprintf(out, "%d\n", min_costs[i]);
        }

        free(min_costs);
        min_costs = NULL;

        // Eliberarea memoriei pentru grafuri
        free_graph(graph);
        for (i = 0; i < nr_components; ++i)
        {
            free(components[i].lists);
        }
        free(components);
    }
    else if (strcmp(argv[1], "2") == 0)
    {
        Digraph graph;
        if (fscanf(in, "%d", &nr_nodes) != 1)
        {
            fprintf(stderr, "Error when reading nr of nodes!\n");
            exit(1);
        }
        if (fscanf(in, "%d", &nr_edges) != 1)
        {
            fprintf(stderr, "Error when reading nr of edges!\n");
            exit(1);
        }
        char(*names1)[MAX_STRLEN] = (char(*)[MAX_STRLEN])
            malloc(nr_edges * sizeof(char[MAX_STRLEN]));
        char(*names2)[MAX_STRLEN] = (char(*)[MAX_STRLEN])
            malloc(nr_edges * sizeof(char[MAX_STRLEN]));
        int *distances = (int *)malloc(nr_edges * sizeof(int));
        int *depths = (int *)malloc(nr_nodes * sizeof(int));
        int treasure_weight;
        char(*nodes)[MAX_STRLEN] = (char(*)[MAX_STRLEN])
            malloc(nr_nodes * sizeof(char[MAX_STRLEN]));

        treasure_weight = read_map(&in, names1, names2, distances, depths,
                                   nodes, nr_nodes, nr_edges);
        graph = init_digraph(&graph, nr_nodes,
                             nr_edges, treasure_weight, nodes);
        graph = build_digraph(graph, names1, names2, distances, depths);

        free(names1);
        free(names2);
        free(distances);
        free(depths);
        free(nodes);

        char(*path)[MAX_STRLEN] = (char(*)[MAX_STRLEN])
            malloc(nr_nodes * sizeof(char[MAX_STRLEN]));
        int path_length;
        bool found;
        found = Dijkstra(graph, "Corabie", "Insula", path, &path_length);
        if (found == false)
        {
            fprintf(out, "Echipajul nu poate ajunge la insula\n");
        }
        else
        {
            found = Dijkstra(graph, "Insula", "Corabie", path, &path_length);
            if (found == false)
            {
                fprintf(out, "Echipajul nu poate transporta comoara inapoi la corabie\n");
            }
            else
            {
                printf("Path: ");
                for (i = 0; i < path_length; ++i)
                {
                    printf("%s ", path[i]);
                }

                int min_depth = INFINITY;
                int total_distance = 0;
                int nr_passes = 0;
                calculate_values(graph, path, path_length,
                                 &min_depth, &total_distance, &nr_passes);

                for (i = 0; i < path_length; ++i)
                {
                    fprintf(out, "%s ", path[i]);
                }
                fprintf(out, "\n%d", total_distance);
                fprintf(out, "\n%d", min_depth);
                fprintf(out, "\n%d\n", nr_passes);
            }
        }
        free(path);
        free_digraph(graph);
    }
    else
    {
        printf("Invalid task number, enter 1 or 2!\n");
        exit(1);
    }

    fclose(in);
    fclose(out);

    return 0;
}

void free_graph(Graph my_graph)
{
    for (int i = 0; i < my_graph.nr_nodes; ++i)
    {
        Node *current = my_graph.lists[i];
        while (current != NULL)
        {
            Node *next = current->next;
            free(current);
            current = next;
        }
    }
    free(my_graph.lists);
}

void free_digraph(Digraph my_graph)
{
    for (int i = 0; i < my_graph.nr_nodes; ++i)
    {
        dirNode *current = my_graph.lists[i];
        while (current != NULL)
        {
            dirNode *next = current->next;
            free(current);
            current = next;
        }
    }
    free(my_graph.lists);
}