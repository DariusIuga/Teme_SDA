#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COST 1000000
#define MAX_STRLEN 12

// O lista inlantuita
typedef struct node
{
    char node_name[MAX_STRLEN];
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

void read_edges(FILE **in, char names1[][MAX_STRLEN], char names2[][MAX_STRLEN], int *weights, int nr_edges);
Graph init_graph(Graph *my_graph, int nr_nodes, int nr_edges);
Graph set_node_names(Graph my_graph, char names1[][MAX_STRLEN], char names2[][MAX_STRLEN]);
int find_node_index(Graph my_graph, char *node_name);
Graph insert_edge(Graph my_graph, char *name1, char *name2, int cost);
Graph build_graph(Graph my_graph, char names1[][MAX_STRLEN], char names2[][MAX_STRLEN], int *costs);
void print_graph(Graph my_graph);

// Subpunctul a
int DFS(Graph my_graph, int node_index, char *visited, int *nr_edges);
int count_connected_components(Graph my_graph);
Graph *find_connected_components(Graph my_graph, int num_components);

// Subpunctul b
int calculate_mst_cost(Graph component);
int cmp_int(const void *a, const void *b);

void read_edges(FILE **in, char names1[][MAX_STRLEN], char names2[][MAX_STRLEN], int *weights, int nr_edges)
{
    int i;
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
        if ((fscanf(*in, "%d", &weights[i])) != 1)
        {
            fprintf(stderr, "Error\n");
        }

        fgetc(*in);
    }
}

Graph init_graph(Graph *my_graph, int nr_nodes, int nr_edges)
{
    int i;
    my_graph->nr_nodes = nr_nodes;
    my_graph->nr_edges = nr_edges;
    my_graph->lists = (Node **)malloc(my_graph->nr_edges * sizeof(Node *));
    for (i = 0; i < nr_nodes; ++i)
    {
        my_graph->lists[i] = (Node *)malloc(sizeof(Node));
    }

    return *my_graph;
}

Graph set_node_names(Graph my_graph, char names1[][MAX_STRLEN], char names2[][MAX_STRLEN])
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

    int i = find_node_index(my_graph, name1);
    strcpy(new->node_name, name2);
    new->cost = cost;

    if (i != -1)
    {
        new->next = my_graph.lists[i]->next;
        my_graph.lists[i]->next = new;
    }

    return my_graph;
}

Graph build_graph(Graph my_graph, char names1[][MAX_STRLEN], char names2[][MAX_STRLEN], int *costs)
{
    int i;
    for (i = 0; i < my_graph.nr_edges; ++i)
    {
        my_graph = insert_edge(my_graph, names1[i], names2[i], costs[i]);
        my_graph = insert_edge(my_graph, names2[i], names1[i], costs[i]);
    }

    return my_graph;
}

// Functie folosita in special pentru debugging
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
        printf("\n\n");
    }
}

// Returneaza nr de noduri din componenta conexa
// in care se afla nodul initial
int DFS(Graph my_graph, int node_index, char *visited, int *nr_edges)
{
    visited[node_index] = 1;

    int count = 1;

    Node *current = my_graph.lists[node_index]->next;
    while (current != NULL)
    {
        int neighbor_index = find_node_index(my_graph, current->node_name);
        if (visited[neighbor_index] == 0)
        {
            count += DFS(my_graph, neighbor_index, visited, nr_edges);
        }
        current = current->next;
        ++(*nr_edges);
    }

    return count;
}

int count_connected_components(Graph my_graph)
{
    int nr_edges = 0, i;
    char *visited = (char *)malloc(my_graph.nr_nodes * sizeof(char));
    for (i = 0; i < my_graph.nr_nodes; ++i)
    {
        visited[i] = 0;
    }

    int count = 0;
    for (i = 0; i < my_graph.nr_nodes; ++i)
    {
        if (visited[i] == 0)
        {
            DFS(my_graph, i, visited, &nr_edges);
            ++count;
        }
    }

    if (visited != NULL)
    {
        free(visited);
        visited = NULL;
    }
    return count;
}

Graph *find_connected_components(Graph my_graph, int nr_components)
{
    char *visited = (char *)malloc(my_graph.nr_nodes * sizeof(char));
    int i, j, k;
    // int node_index;
    int component_size = 0;
    for (i = 0; i < my_graph.nr_nodes; ++i)
    {
        visited[i] = 0;
    }

    Graph *components = (Graph *)malloc(nr_components * sizeof(Graph));

    for (i = 0; i < nr_components; ++i)
    {
        // Gasim indexii din componenta respectiva, nr de noduri si muchii
        int nr_edges = 0;
        // Node *temp = (Node *)malloc(sizeof(Node));

        for (j = 0; j < my_graph.nr_nodes; ++j)
        {
            if (visited[j] == 0)
            {
                component_size = DFS(my_graph, j, visited, &nr_edges);
                nr_edges /= 2;
                break;
            }
        }

        // Intializam componenta i
        components[i].nr_nodes = component_size;
        components[i].nr_edges = nr_edges;
        components[i].lists = (Node **)malloc(my_graph.nr_edges * sizeof(Node *));

        k = 0;
        for (j = 0; j < my_graph.nr_nodes; ++j)
        {
            // Avem 3 cazuri:
            // 1) visited[j] == 0 => Nodul nu a fost vizitat.
            // 2) visited[j] == 1 => Nodul a fost vizitat
            // si va fi adaugat in componenta curenta.
            // 2) visited[j] == 2 => Nodul a fost vizitat dar
            // a fost deja adaugat intr-o alta componenta.

            if (visited[j] == 1)
            {
                components[i].lists[k] = my_graph.lists[j];
                ++visited[j];
                ++k;
            }
        }
    }

    if (visited != NULL)
    {
        free(visited);
        visited = NULL;
    }
    return components;
}

int calculate_mst_cost(Graph component)
{
    int total_cost = 0;
    int i;

    int *visited = (int *)malloc(component.nr_nodes * sizeof(int));
    for (i = 0; i < component.nr_nodes; ++i)
    {
        visited[i] = 0;
    }

    // Nodul din care incepem cautarea va fi primul
    visited[0] = 1;

    // Cautarea se repeta pana cand toate nodurile au fost vizitate
    while (1)
    {
        int min_cost = MAX_COST;
        int min_cost_node = -1;

        // Gaseste muchia de cost minim care conecteaza un nod vizitat
        // de unul nevizitat
        for (i = 0; i < component.nr_nodes; ++i)
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

        // Daca nu se mai gasesc muchii de cost minim, algoritmul e gata.
        if (min_cost_node == -1)
        {
            break;
        }

        // Adauga muchia de cost minim in MST
        // si marcheaza nodul drept vizitat.
        total_cost += min_cost;
        visited[min_cost_node] = 1;
    }

    if (visited != NULL)
    {
        free(visited);
        visited = NULL;
    }
    return total_cost;
}

// Furnizata ca parametru lui qsort pentru a compara costurile drumurilor
int cmp_int(const void *a, const void *b)
{
    int x = *(int *)a;
    int y = *(int *)b;
    return x - y;
}
