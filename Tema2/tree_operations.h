// Acest header contine functiile si structurile comune tuturor headerelor

#ifndef TREE_OPERATIONS_H
#define TREE_OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u_char;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef struct pixel
{
    u_char red, green, blue;
} pixel;

typedef struct point
{
    u32 x, y;
} point;

// Un nod din arborele cuaternar
typedef struct tree_node tree_node;
struct tree_node
{
    tree_node *top_left;
    tree_node *top_right;
    tree_node *bottom_right;
    tree_node *bottom_left;
    // 0==>intern; 1==>frunza;
    u_char type;
    u_char depth;
    u_char red;
    u_char green;
    u_char blue;
    point corner_tl;
    point corner_br;
};

// Valorile retinute in reprezentarea vectoriala a arborelui.
typedef struct data
{
    u_char type;
    u_char depth;
    u_char red;
    u_char green;
    u_char blue;
} data;

// Intreg vectorul prin care este reprezentat arborele.
// length reprezinta nr de elemente retinute,
// iar capacity memoria totala alocata.
typedef struct array
{
    data *array;
    u32 length;
    u32 cap;
} array;

pixel **read_ppm(FILE **in, char *name, u32 *size);
void init_tree(tree_node **root, u32 size);
void init_leaf(tree_node **leaf, tree_node *parent);
void generate_subtrees(tree_node *parent);
void calculate_mean(tree_node *block, pixel **image, u32 size,
                    u64 *R, u64 *G, u64 *B, u64 *mean);
void init_vector(array *vector);
void free_quadtree(tree_node *current);

pixel **read_ppm(FILE **in, char *name, u32 *size)
{
    if ((*in = fopen(name, "rb")) == NULL)
    {
        fprintf(stderr, "Error when opening image ppm file!");
    }
    char type[2];
    u32 max_value, i, j;
    if (fscanf(*in, "%s", type) != 1)
    {
        fprintf(stderr, "Eroare la citirea datelor!\n");
    }
    fgetc(*in);
    if (fscanf(*in, "%u", size) != 1)
    {
        fprintf(stderr, "Eroare la citirea datelor!\n");
    }
    fgetc(*in);
    if (fscanf(*in, "%u", size) != 1)
    {
        fprintf(stderr, "Eroare la citirea datelor!\n");
    }
    fgetc(*in);
    if (fscanf(*in, "%u", &max_value) != 1)
    {
        fprintf(stderr, "Eroare la citirea datelor!\n");
    }
    fgetc(*in);
    pixel **image = (pixel **)malloc(*size * sizeof(pixel *));

    for (i = 0; i < *size; ++i)
    {
        image[i] = (pixel *)malloc(*size * sizeof(pixel));
        for (j = 0; j < *size; ++j)
        {
            if (fread(&image[i][j].red, sizeof(char), 1, *in) != 1)
            {
                fprintf(stderr, "Eroare la citirea datelor!\n");
            }
            if (fread(&image[i][j].green, sizeof(char), 1, *in) != 1)
            {
                fprintf(stderr, "Eroare la citirea datelor!\n");
            }
            if (fread(&image[i][j].blue, sizeof(char), 1, *in) != 1)
            {
                fprintf(stderr, "Eroare la citirea datelor!\n");
            }
        }
    }

    return image;
}

void init_tree(tree_node **root, u32 size)
{
    *root = (tree_node *)malloc(sizeof(tree_node));

    (*root)->type = 1;
    (*root)->corner_tl.x = 0;
    (*root)->corner_tl.y = 0;
    (*root)->corner_br.x = size - 1;
    (*root)->corner_br.y = size - 1;
    (*root)->depth = 0;
    (*root)->top_left = NULL;
    (*root)->top_right = NULL;
    (*root)->bottom_right = NULL;
    (*root)->bottom_left = NULL;
}

void init_leaf(tree_node **leaf, tree_node *parent)
{
    *leaf = (tree_node *)malloc(sizeof(tree_node));

    (*leaf)->type = 1;
    (*leaf)->depth = (parent->depth) + 1;
    (*leaf)->top_left = NULL;
    (*leaf)->top_right = NULL;
    (*leaf)->bottom_right = NULL;
    (*leaf)->bottom_left = NULL;
}

void generate_subtrees(tree_node *parent)
{
    // North-West si South-East
    point NW, SE, mid;
    NW.x = parent->corner_tl.x;
    NW.y = parent->corner_tl.y;
    SE.x = parent->corner_br.x;
    SE.y = parent->corner_br.y;
    mid.x = (NW.x + SE.x) / 2;
    mid.y = (NW.y + SE.y) / 2;

    // Este alocata memorie pentru subarbori
    init_leaf(&parent->top_left, parent);
    init_leaf(&parent->top_right, parent);
    init_leaf(&parent->bottom_right, parent);
    init_leaf(&parent->bottom_left, parent);

    // Nodul parinte devine unul de tip inner
    parent->type = 0;

    // Subarborele din stanga sus:
    parent->top_left->corner_tl.x = NW.x;
    parent->top_left->corner_tl.y = NW.y;
    parent->top_left->corner_br.x = mid.x;
    parent->top_left->corner_br.y = mid.y;

    // Subarborele din dreapta sus:
    parent->top_right->corner_tl.x = mid.x + 1;
    parent->top_right->corner_tl.y = NW.y;
    parent->top_right->corner_br.x = SE.x;
    parent->top_right->corner_br.y = mid.y;

    // Subarborele din dreapta jos:
    parent->bottom_right->corner_tl.x = mid.x + 1;
    parent->bottom_right->corner_tl.y = mid.y + 1;
    parent->bottom_right->corner_br.x = SE.x;
    parent->bottom_right->corner_br.y = SE.y;

    // Subarborele din stanga jos:
    parent->bottom_left->corner_tl.x = NW.x;
    parent->bottom_left->corner_tl.y = mid.y + 1;
    parent->bottom_left->corner_br.x = mid.x;
    parent->bottom_left->corner_br.y = SE.y;
}

void calculate_mean(tree_node *current, pixel **image, u32 size,
                    u64 *R, u64 *G, u64 *B, u64 *mean)
{
    u32 i, j;

    for (i = current->corner_tl.x; i <= current->corner_br.x; ++i)
    {
        for (j = current->corner_tl.y; j <= current->corner_br.y; ++j)
        {
            *R += image[j][i].red;
            *G += image[j][i].green;
            *B += image[j][i].blue;
        }
    }
    *R /= size * size;
    *G /= size * size;
    *B /= size * size;

    for (i = current->corner_tl.x; i <= current->corner_br.x; ++i)
    {
        for (j = current->corner_tl.y; j <= current->corner_br.y; ++j)
        {
            *mean += (*R - image[j][i].red) * (*R - image[j][i].red) +
                     (*G - image[j][i].green) * (*G - image[j][i].green) +
                     (*B - image[j][i].blue) * (*B - image[j][i].blue);
        }
    }
    *mean /= 3 * size * size;
}

void init_vector(array *vector)
{
    vector->array = (data *)malloc(sizeof(data));
    vector->length = 0;
    vector->cap = 1;
}

void free_quadtree(tree_node *current)
{
    // Conditie de oprire: nodul este o frunza
    if (current == NULL)
    {
        return;
    }

    free_quadtree(current->top_left);
    free_quadtree(current->top_right);
    free_quadtree(current->bottom_right);
    free_quadtree(current->bottom_left);

    free(current);
}

#endif