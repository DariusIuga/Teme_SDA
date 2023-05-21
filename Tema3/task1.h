#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COST 1000000

// O lista inlantuita
typedef struct node
{
    char node_name[20];
    int cost;
    struct node *next;
} Node;

// Reprezentarea grafului ponderat prin liste inlantuite
typedef struct graph
{
    int nr_nodes;
    int nr_edges;
    Node **lists;
} Graph;

void read_edges(FILE **in, char **names1, char **names2, int *weights, int nr_edges);
Graph init_graph(Graph my_graph, int nr_nodes, int nr_edges);
Graph set_node_names(Graph my_graph, char **names1, char **names2);
int find_node_index(Graph my_graph, char *node_name);
Graph insert_edge(Graph my_graph, char *name1, char *name2, int cost);
Graph build_graph(Graph my_graph, char **names1, char **names2, int *costs);
void print_graph(Graph my_graph);

int DFS(Graph my_graph, int node_index, char *visited);
int count_connected_components(Graph my_graph);
Graph *represent_connected_components(Graph my_graph, int *num_components);
int calculate_mst_cost(Graph component);
int int_cmp(const void *a, const void *b);

void read_edges(FILE **in, char **names1, char **names2, int *weights, int nr_edges)
{
    int i;
    for (i = 0; i < nr_edges; ++i)
    {
        fscanf(*in, "%s", names1[i]);
        fscanf(*in, "%s", names2[i]);
        fscanf(*in, "%d", &weights[i]);
        fgetc(*in);
    }
}

Graph init_graph(Graph my_graph, int nr_nodes, int nr_edges)
{
    int i;
    my_graph.nr_nodes = nr_nodes;
    my_graph.nr_edges = nr_edges;
    for (i = 0; i < nr_nodes; ++i)
    {
        my_graph.lists[i] = (Node *)malloc(sizeof(Node));
    }

    return my_graph;
}

Graph set_node_names(Graph my_graph, char **names1, char **names2)
{
    int i, found_names = 0, j;
    char not_seen;

    // Parcurgem primul vector cu nume
    for (i = 0; i < my_graph.nr_edges; ++i)
    {
        if (found_names == 0)
        {
            strcpy(my_graph.lists[found_names]->node_name, names1[i]);
            my_graph.lists[found_names]->cost = 0;
            ++found_names;
        }
        else
        {
            not_seen = 1;
            for (j = 0; j < found_names; ++j)
            {
                if (strcmp(my_graph.lists[j]->node_name, names1[i]) == 0)
                {
                    not_seen = 0;
                }
            }
            if (not_seen == 1)
            {
                strcpy(my_graph.lists[found_names]->node_name, names1[i]);
                my_graph.lists[found_names]->cost = 0;
                ++found_names;
            }
        }
    }

    // Parcurgem al doilea vector cu nume
    // Pot fi noduri de gradul 1 care sunt legate de nodurile
    // cu numele citite in primul vector
    for (i = 0; i < my_graph.nr_edges; ++i)
    {
        not_seen = 1;
        for (j = 0; j < found_names; ++j)
        {
            if (strcmp(my_graph.lists[j]->node_name, names2[i]) == 0)
            {
                not_seen = 0;
            }
        }
        if (not_seen == 1)
        {
            strcpy(my_graph.lists[found_names]->node_name, names2[i]);
            ++found_names;
        }
    }

    return my_graph;
}

int find_node_index(Graph my_graph, char *node_name)
{
    int i;
    for (i = 0; i < my_graph.nr_nodes; ++i)
    {
        if (strcmp(my_graph.lists[i]->node_name, node_name) == 0)
        {
            return i;
        }
    }

    // Nodul nu a fost gasit
    return -1;
}

Graph insert_edge(Graph my_graph, char *name1, char *name2, int cost)
{
    Node *new = (Node *)malloc(sizeof(Node));
    Node *temp = (Node *)malloc(sizeof(Node));

    int i = find_node_index(my_graph, name1);
    strcpy(new->node_name, name2);
    new->cost = cost;

    if (i != -1)
    {
        temp = my_graph.lists[i]->next;
        my_graph.lists[i]->next = new;
        new->next = temp;
    }

    return my_graph;
}

Graph build_graph(Graph my_graph, char **names1, char **names2, int *costs)
{
    int i;
    for (i = 0; i < my_graph.nr_edges; ++i)
    {
        my_graph = insert_edge(my_graph, names1[i], names2[i], costs[i]);
        my_graph = insert_edge(my_graph, names2[i], names1[i], costs[i]);
    }

    return my_graph;
}

void print_graph(Graph my_graph)
{
    int i;
    Node *current = (Node *)malloc(sizeof(Node));

    printf("Nr noduri: %d\n", my_graph.nr_nodes);
    printf("Nr muchii: %d\n\n", my_graph.nr_edges);
    printf("Lista de adiacenta:\n\n");

    for (i = 0; i < my_graph.nr_nodes; ++i)
    {
        current = my_graph.lists[i];
        printf("%s: ", current->node_name);

        while (current->next != NULL)
        {
            current = current->next;
            printf("%s ", current->node_name);
            printf("%d ", current->cost);
        }
        printf("\n");
    }
}

int DFS(Graph my_graph, int node_index, char *visited)
{
    visited[node_index] = 1;
    int count = 1;

    Node *current = my_graph.lists[node_index]->next;
    while (current != NULL)
    {
        int neighbor_index = find_node_index(my_graph, current->node_name);
        if (visited[neighbor_index] == 0)
            count += DFS(my_graph, neighbor_index, visited);
        current = current->next;
    }

    return count;
}

int count_connected_components(Graph my_graph)
{
    char *visited = (char *)malloc(my_graph.nr_nodes * sizeof(char));
    for (int i = 0; i < my_graph.nr_nodes; ++i)
    {
        visited[i] = 0;
    }

    int count = 0;
    for (int i = 0; i < my_graph.nr_nodes; ++i)
    {
        if (visited[i] == 0)
        {
            int component_size = DFS(my_graph, i, visited);
            if (component_size > 0)
            {
                ++count;
            }
        }
    }

    free(visited);
    return count;
}

Graph *represent_connected_components(Graph my_graph, int *num_components)
{
    char *visited = (char *)malloc(my_graph.nr_nodes * sizeof(char));
    for (int i = 0; i < my_graph.nr_nodes; ++i)
        visited[i] = 0;

    *num_components = count_connected_components(my_graph);

    Graph *components = (Graph *)malloc((*num_components) * sizeof(Graph));
    for (int i = 0; i < *num_components; ++i)
    {
        components[i].nr_nodes = 0;
        components[i].nr_edges = 0;
        components[i].lists = (Node **)malloc(my_graph.nr_nodes * sizeof(Node *));
        for (int j = 0; j < my_graph.nr_nodes; ++j)
            components[i].lists[j] = NULL;
    }

    int component_index = 0;
    for (int i = 0; i < my_graph.nr_nodes; ++i)
    {
        if (visited[i] == 0)
        {
            int component_size = DFS(my_graph, i, visited);
            if (component_size > 0)
            {
                components[component_index].nr_nodes = component_size;
                components[component_index].lists = (Node **)malloc(component_size * sizeof(Node *));
                for (int j = 0; j < component_size; ++j)
                    components[component_index].lists[j] = NULL;

                int node_index = 0;
                for (int j = 0; j < my_graph.nr_nodes; ++j)
                {
                    if (visited[j] == 1)
                    {
                        components[component_index].lists[node_index] = (Node *)malloc(sizeof(Node));
                        strcpy(components[component_index].lists[node_index]->node_name, my_graph.lists[j]->node_name);
                        components[component_index].lists[node_index]->cost = my_graph.lists[j]->cost;
                        ++node_index;
                    }
                }

                ++component_index;
            }
        }
    }

    free(visited);
    return components;
}

int calculate_mst_cost(Graph component)
{
    int total_cost = 0;

    // Create a visited array to track visited nodes
    int *visited = (int *)malloc(component.nr_nodes * sizeof(int));
    for (int i = 0; i < component.nr_nodes; ++i)
        visited[i] = 0;

    // Select the starting node as the first node in the component
    int start_node = 0;
    visited[start_node] = 1;

    // Repeat until all nodes are visited
    while (1)
    {
        int min_cost = MAX_COST;
        int min_cost_node = -1;

        // Find the minimum cost edge connecting a visited node to an unvisited node
        for (int i = 0; i < component.nr_nodes; ++i)
        {
            if (visited[i] == 1)
            {
                Node *current = component.lists[i]->next;
                while (current != NULL)
                {
                    int neighbor_index = find_node_index(component, current->node_name);
                    if (visited[neighbor_index] == 0 && current->cost < min_cost)
                    {
                        min_cost = current->cost;
                        min_cost_node = neighbor_index;
                    }
                    current = current->next;
                }
            }
        }

        // If no minimum cost edge is found, the MST is complete
        if (min_cost_node == -1)
            break;

        // Add the minimum cost edge to the MST and mark the node as visited
        total_cost += min_cost;
        visited[min_cost_node] = 1;
    }

    // Cleanup: free memory allocated for the visited array
    free(visited);

    return total_cost;
}

int int_cmp(const void *a, const void *b)
{
    int x = *(int *)a;
    int y = *(int *)b;
    return x - y;
}