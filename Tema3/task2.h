#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRLEN 12
#define INFINITY 10000

typedef struct directed_node
{
    char node_name[MAX_STRLEN];
    int distance;
    int depth;
    float score;
    struct directed_node *next;
} dirNode;

typedef struct directed_graph
{
    int nr_nodes;
    int nr_edges;
    int treasure_weight;
    dirNode **lists;
} Digraph;

int read_map(FILE **in, char names1[][MAX_STRLEN], char names2[][MAX_STRLEN],
             int *distances, int *depths, char nodes[][MAX_STRLEN],
             int nr_edges, int nr_nodes);

Digraph init_digraph(Digraph *graph,
                     int nr_nodes, int nr_edges,
                     int treasure_weight, char nodes[][MAX_STRLEN]);

int find_dirNode_index(Digraph graph, char *node_name);

Digraph insert_digraph_edge(Digraph graph, char *name1,
                            char *name2, int distance, int depth);

Digraph build_digraph(Digraph graph, char names1[][MAX_STRLEN],
                      char names2[][MAX_STRLEN], int *distances, int *depths);

void print_digraph(Digraph graph);

int read_map(FILE **in, char names1[][MAX_STRLEN], char names2[][MAX_STRLEN],
             int *distances, int *depths, char nodes[][MAX_STRLEN],
             int nr_nodes, int nr_edges)
{
    int i;
    int treasure_weight;
    for (i = 0; i < nr_edges; ++i)
    {
        if ((fscanf(*in, "%s", names1[i])) != 1)
        {
            fprintf(stderr, "Error\n");
        }
        if ((fscanf(*in, "%s", names2[i])) != 1)
        {
            fprintf(stderr, "Error\n");
        }
        if ((fscanf(*in, "%d", &distances[i])) != 1)
        {
            fprintf(stderr, "Error\n");
        }

        fgetc(*in);
    }
    for (i = 0; i < nr_nodes; ++i)
    {
        if ((fscanf(*in, "%s", nodes[i])) != 1)
        {
            fprintf(stderr, "Error\n");
        }
        if ((fscanf(*in, "%d", &depths[i])) != 1)
        {
            fprintf(stderr, "Error\n");
        }

        fgetc(*in);
    }

    if ((fscanf(*in, "%d", &treasure_weight)) != 1)
    {
        fprintf(stderr, "Error\n");
    }

    return treasure_weight;
}

Digraph init_digraph(Digraph *graph,
                     int nr_nodes, int nr_edges,
                     int treasure_weight, char nodes[][MAX_STRLEN])
{
    int i;
    graph->nr_nodes = nr_nodes;
    graph->nr_edges = nr_edges;
    graph->treasure_weight = treasure_weight;
    graph->lists = (dirNode **)malloc(graph->nr_edges * sizeof(dirNode *));
    for (i = 0; i < nr_nodes; ++i)
    {
        graph->lists[i] = (dirNode *)malloc(sizeof(dirNode));
        strcpy(graph->lists[i]->node_name, nodes[i]);
        graph->lists[i]->next = NULL;
    }

    return *graph;
}

int find_dirNode_index(Digraph graph, char *node_name)
{
    int i;
    for (i = 0; i < graph.nr_nodes; ++i)
    {
        if (strcmp(graph.lists[i]->node_name, node_name) == 0)
        {
            return i;
        }
    }

    // Nodul nu a fost gasit
    return -1;
}

Digraph insert_digraph_edge(Digraph graph, char *name1,
                            char *name2, int distance, int depth)
{
    dirNode *new = (dirNode *)malloc(sizeof(dirNode));

    int i = find_dirNode_index(graph, name1);
    strcpy(new->node_name, name2);
    new->distance = distance;
    new->depth = depth;
    new->score = (float)distance / depth;

    if (i != -1)
    {
        new->next = graph.lists[i]->next;
        graph.lists[i]->next = new;
    }

    return graph;
}

Digraph build_digraph(Digraph graph, char names1[][MAX_STRLEN],
                      char names2[][MAX_STRLEN], int *distances, int *depths)
{
    int i;
    int destination;
    for (i = 0; i < graph.nr_edges; ++i)
    {
        destination = find_dirNode_index(graph, names2[i]);
        graph = insert_digraph_edge(graph, names1[i], names2[i], distances[i], depths[destination]);
    }

    return graph;
}

void print_digraph(Digraph graph)
{
    int i;
    dirNode *current = (dirNode *)malloc(sizeof(dirNode));

    printf("Nr noduri: %d\n", graph.nr_nodes);
    printf("Nr muchii: %d\n\n", graph.nr_edges);
    printf("Lista de adiacenta:\n\n");

    for (i = 0; i < graph.nr_nodes; ++i)
    {
        current = graph.lists[i];
        printf("%s: ", current->node_name);

        while (current->next != NULL)
        {
            current = current->next;
            printf("Nume: %s ", current->node_name);
            printf("Distanta: %d ", current->distance);
            printf("Adancime: %d ", current->depth);
            printf("Scor: %f |", current->score);
        }
        printf("\n\n");
    }
}

// void initialize_distances(Digraph *graph, int source_index)
// {
//     int i;
//     for (i = 0; i < graph->nr_nodes; ++i)
//     {
//         if (i == source_index)
//         {
//             graph->lists[i]->distance = 0;
//             graph->lists[i]->depth = 0;
//             graph->lists[i]->score = 0.0f;
//         }
//         else
//         {
//             graph->lists[i]->distance = INFINITY;
//             graph->lists[i]->depth = INFINITY;
//             graph->lists[i]->score = INFINITY;
//         }
//     }
// }

// int get_min_distance(Digraph *graph, int *visited)
// {
//     int i, min_index;
//     float min_score = INFINITY;

//     for (i = 0; i < graph->nr_nodes; ++i)
//     {
//         if (visited[i] == 0 && graph->lists[i]->score < min_score)
//         {
//             min_score = graph->lists[i]->score;
//             min_index = i;
//         }
//     }

//     return min_index;
// }

// void update_neighbors(Digraph *graph, int current_index)
// {
//     dirNode *current_node = graph->lists[current_index];
//     dirNode *neighbor = current_node->next;

//     while (neighbor != NULL)
//     {
//         int neighbor_index = find_dirNode_index(*graph, neighbor->node_name);
//         if (neighbor_index != -1)
//         {
//             float new_score = current_node->score + neighbor->score;

//             if (new_score < graph->lists[neighbor_index]->score)
//             {
//                 graph->lists[neighbor_index]->distance = current_node->distance + 1;
//                 graph->lists[neighbor_index]->depth = current_node->depth + 1;
//                 graph->lists[neighbor_index]->score = new_score;
//             }
//         }

//         neighbor = neighbor->next;
//     }
// }

// int Dijkstra(Digraph graph, char source[MAX_STRLEN], char dest[MAX_STRLEN], char path[][MAX_STRLEN])
// {
//     int source_index = find_dirNode_index(graph, source);
//     int dest_index = find_dirNode_index(graph, dest);

//     if (source_index == -1 || dest_index == -1)
//     {
//         printf("Invalid source or destination node.\n");
//         return -1;
//     }

//     int *distances = (int *)malloc(graph.nr_nodes * sizeof(int));
//     int *previous = (int *)malloc(graph.nr_nodes * sizeof(int));
//     bool *visited = (bool *)malloc(graph.nr_nodes * sizeof(bool));

//     // Initialize distances and visited array
//     for (int i = 0; i < graph.nr_nodes; i++)
//     {
//         distances[i] = INFINITY;
//         visited[i] = false;
//         previous[i] = -1;
//     }

//     distances[source_index] = 0;

//     for (int count = 0; count < graph.nr_nodes - 1; count++)
//     {
//         int min_distance = INFINITY;
//         int min_index = -1;

//         // Find the node with the minimum distance
//         for (int i = 0; i < graph.nr_nodes; i++)
//         {
//             if (!visited[i] && distances[i] < min_distance)
//             {
//                 min_distance = distances[i];
//                 min_index = i;
//             }
//         }

//         if (min_index == -1)
//         {
//             break;
//         }

//         visited[min_index] = true;

//         // Update distances of adjacent nodes
//         dirNode *current = graph.lists[min_index];
//         while (current != NULL)
//         {
//             int neighbor_index = find_dirNode_index(graph, current->node_name);
//             if (neighbor_index != -1)
//             {
//                 int new_distance = distances[min_index] + current->score;
//                 if (new_distance < distances[neighbor_index])
//                 {
//                     distances[neighbor_index] = new_distance;
//                     previous[neighbor_index] = min_index;
//                 }
//             }
//             current = current->next;
//         }
//     }

//     int path_index = 0;
//     int current_index = dest_index;

//     // Build the path in reverse order
//     while (current_index != -1)
//     {
//         strcpy(path[path_index], graph.lists[current_index]->node_name);
//         current_index = previous[current_index];
//         path_index++;
//     }

//     // Reverse the path to get the correct order
//     for (int i = 0, j = path_index - 1; i < j; i++, j--)
//     {
//         char temp[MAX_STRLEN];
//         strcpy(temp, path[i]);
//         strcpy(path[i], path[j]);
//         strcpy(path[j], temp);
//     }

//     free(distances);
//     free(previous);
//     free(visited);

//     return path_index;
// }

bool Dijkstra(Digraph graph, char source[MAX_STRLEN], char dest[MAX_STRLEN], char path[][MAX_STRLEN], int *path_length)
{
    int num_nodes = graph.nr_nodes;
    int *distances = (int *)malloc(num_nodes * sizeof(int));
    int *previous = (int *)malloc(num_nodes * sizeof(int));
    bool *visited = (bool *)malloc(num_nodes * sizeof(bool));

    // Initialize distances, previous nodes, and visited array
    int i;
    for (i = 0; i < num_nodes; i++)
    {
        distances[i] = INFINITY;
        previous[i] = -1;
        visited[i] = false;
    }

    // Find the indices of the source and destination nodes
    int source_index = find_dirNode_index(graph, source);
    int dest_index = find_dirNode_index(graph, dest);

    if (source_index == -1 || dest_index == -1)
    {
        free(distances);
        free(previous);
        free(visited);
        return false; // One of the nodes is not found in the graph
    }

    distances[source_index] = 0;

    // Dijkstra's algorithm
    int current_node;
    while (true)
    {
        int min_distance = INFINITY;
        current_node = -1;

        // Find the unvisited node with the smallest distance
        for (i = 0; i < num_nodes; i++)
        {
            if (!visited[i] && distances[i] < min_distance)
            {
                min_distance = distances[i];
                current_node = i;
            }
        }

        if (current_node == -1)
            break; // No path found

        visited[current_node] = true;

        // Update distances of neighboring nodes
        dirNode *node = graph.lists[current_node]->next;
        while (node != NULL)
        {
            int neighbor_index = find_dirNode_index(graph, node->node_name);
            int new_distance = distances[current_node] + node->distance;

            if (new_distance < distances[neighbor_index])
            {
                distances[neighbor_index] = new_distance;
                previous[neighbor_index] = current_node;
            }

            node = node->next;
        }
    }

    // Reconstruct the shortest path
    *path_length = 0;
    current_node = dest_index;
    while (current_node != -1)
    {
        strcpy(path[*path_length], graph.lists[current_node]->node_name);
        (*path_length)++;
        current_node = previous[current_node];
    }

    // Reverse the path array
    int start = 0;
    int end = (*path_length) - 1;
    while (start < end)
    {
        char temp[MAX_STRLEN];
        strcpy(temp, path[start]);
        strcpy(path[start], path[end]);
        strcpy(path[end], temp);
        start++;
        end--;
    }

    free(distances);
    free(previous);
    free(visited);

    if (*path_length > 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void calculate_values(Digraph graph, char path[][MAX_STRLEN], int path_length,
                      int *min_depth, int *total_distance, int *nr_passes)
{
    int i, index;
    dirNode *current;
    *min_depth = INFINITY;
    *total_distance = 0;
    for (i = 0; i < path_length - 1; ++i)
    {
        index = find_dirNode_index(graph, path[i]);
        current = graph.lists[index];
        while (strcmp(current->node_name, path[i + 1]) != 0)
        {
            current = current->next;
        }

        // Nu ia in calcul adancimea corabiei
        if (current->depth < (*min_depth) && i < path_length - 2)
        {
            *min_depth = current->depth;
        }
        *total_distance += current->distance;
    }

    *nr_passes = graph.treasure_weight / *min_depth;
    if (graph.treasure_weight % *min_depth != 0)
    {
        ++(*nr_passes);
    }
}