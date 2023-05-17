#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    char node_name[20];
    int cost;
    struct node *next;
} Node, *List;

typedef struct graph
{
    int nr_nodes;
    int nr_edges;
    List *lists;
} Graph;

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
        my_graph.lists[i] = NULL;
    }

    return my_graph;
}

Graph set_node_names(Graph my_graph, char **names1, char **names2)
{
    int i, found = 0, j;
    char new;

    // Parcurgem primul vector cu nume
    for (i = 0; i < my_graph.nr_edges; ++i)
    {
        if (found == 0)
        {
            strcpy(my_graph.lists[found]->node_name, names1[i]);
            ++found;
        }
        else
        {
            new = 1;
            for (j = 0; j < found; ++j)
            {
                if (strcmp(my_graph.lists[j]->node_name, names1[i]) == 0)
                {
                    new = 0;
                }
            }
            if (new == 1)
            {
                strcpy(my_graph.lists[found]->node_name, names1[i]);
                ++found;
            }
        }
    }

    // Parcurgem al doilea vector cu nume
    for (i = 0; i < my_graph.nr_edges; ++i)
    {
        new = 1;
        for (j = 0; j < found; ++j)
        {
            if (strcmp(my_graph.lists[j]->node_name, names2[i]) == 0)
            {
                new = 0;
            }
        }
        if (new == 1)
        {
            strcpy(my_graph.lists[found]->node_name, names2[i]);
            ++found;
        }
    }

    return my_graph;
}

Graph insert_edge(Graph my_graph, char *name1, char *name2, int cost)
{
    List new = (List)malloc(sizeof(Node));
    int i;
    strcpy(new->node_name, name2);
    new->cost = cost;

    for (i = 0; i < my_graph.nr_nodes; ++i)
    {
        if (strcmp(my_graph.lists[i]->node_name, name1) == 0)
        {
            new->next = my_graph.lists[i];
            my_graph.lists[i] = new;
        }
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