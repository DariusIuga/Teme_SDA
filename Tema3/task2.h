#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRLEN 12
#define INFINITY 10000

// Un nod din graful orientat si lista sa de adiacenta
typedef struct directed_node
{
    char node_name[MAX_STRLEN];
    int distance;
    int depth;
    float score;
    struct directed_node *next;
} dirNode;

// Intregul graf orientat.
// Am pus si greutatea comorii pt ca nu depinde de un anumit nod.
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

// Multe dintre functiile acestea sunt adaptate de la graful neorientat
Digraph init_digraph(Digraph *graph,
                     int nr_nodes, int nr_edges,
                     int treasure_weight, char nodes[][MAX_STRLEN]);

int find_dirNode_index(Digraph graph, char *node_name);

Digraph insert_digraph_edge(Digraph graph, char *name1,
                            char *name2, int distance, int depth);

Digraph build_digraph(Digraph graph, char names1[][MAX_STRLEN],
                      char names2[][MAX_STRLEN], int *distances, int *depths);

void print_digraph(Digraph graph);

bool Dijkstra(Digraph graph, char source[MAX_STRLEN], char dest[MAX_STRLEN],
              char path[][MAX_STRLEN], int *path_length);

void calculate_values(Digraph graph, char path[][MAX_STRLEN], int path_length,
                      int *min_depth, int *total_distance, int *nr_passes);

// Citeste valorile folosite pentru a reprezenta graful si harta in general
// din "tema3.out"
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
        // Am citit deja in mod unic numele tuturor nodurilor,
        // deci pot sa le asociez direct de cate o lista de adiacenta.
        strcpy(graph->lists[i]->node_name, nodes[i]);
        graph->lists[i]->next = NULL;
    }

    return *graph;
}

// Nu am codificat numele nodurilor, deci caut indexul pentru fiecare.
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
        // Fiind graf orientat, adaug muchia intr-un singur sens.
        graph = insert_digraph_edge(graph, names1[i], names2[i],
                                    distances[i], depths[destination]);
    }

    return graph;
}

// Folosita pentru debugging
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

// Gaseste calea de cost minim sub forma unui vector de nume pt
// nodurile din acea cale
bool Dijkstra(Digraph graph, char source[MAX_STRLEN], char dest[MAX_STRLEN],
              char path[][MAX_STRLEN], int *path_length)
{
    int num_nodes = graph.nr_nodes;
    int *distances = (int *)malloc(num_nodes * sizeof(int));
    int *previous = (int *)malloc(num_nodes * sizeof(int));
    bool *visited = (bool *)malloc(num_nodes * sizeof(bool));

    // Initializez distatele cu o valoare foarte mare, nodurile precedente,
    // si un vector cu nodurile vizitate.
    int i;
    for (i = 0; i < num_nodes; i++)
    {
        distances[i] = INFINITY;
        previous[i] = -1;
        visited[i] = false;
    }

    // Gasesc indecsii pt nodurile sursa si destinatie.
    int source_index = find_dirNode_index(graph, source);
    int dest_index = find_dirNode_index(graph, dest);

    // Unul dintre noduri nu se gaseste in graf
    if (source_index == -1 || dest_index == -1)
    {
        free(distances);
        free(previous);
        free(visited);
        return false;
    }

    distances[source_index] = 0;

    // Algoritmul lui Djikstra
    int current_node;
    while (true)
    {
        int min_distance = INFINITY;
        current_node = -1;

        // Gaseste nodul nevizitat cu valoarea cea mai mica
        for (i = 0; i < num_nodes; ++i)
        {
            if (!visited[i] && distances[i] < min_distance)
            {
                min_distance = distances[i];
                current_node = i;
            }
        }

        if (current_node == -1)
        {
            break; // Nicio cale gasita
        }
        visited[current_node] = true;

        // Actualizez distantele nodurilor vecine
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

    // Adaug numele nodurilor din calea optima in vectorul path.
    *path_length = 0;
    current_node = dest_index;
    while (current_node != -1)
    {
        strcpy(path[*path_length], graph.lists[current_node]->node_name);
        (*path_length)++;
        // Fac backtracking de la destinatie la sursa
        current_node = previous[current_node];
    }

    // Acum ordinea numelor trebuie inversata.
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

    // Eliberez memoria
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