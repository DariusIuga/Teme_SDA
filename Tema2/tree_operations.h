// Acest header contine functiile si structurile comune tuturor headerelor

#ifndef TREE_OPERATIONS_H
#define TREE_OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int u32;
typedef unsigned long long u64;

typedef struct pixel
{
    unsigned char red, green, blue;
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
    unsigned char type;
    unsigned char depth;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    point corner_top_left;
    point corner_bottom_right;
};

// Valorile retinute in reprezentarea vectoriala a arborelui.
typedef struct cell_data
{
    unsigned char type;
    unsigned char depth;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} cell_data;

// Intreg vectorul prin care este reprezentat arborele.
// length reprezinta nr de elemente retinute,
// iar capacity memoria totala alocata.
typedef struct cell_array
{
    cell_data *array;
    u32 length;
    u32 capacity;
} cell_array;

pixel **read_ppm(FILE **in, char *name, u32 *size);
void init_tree(tree_node **root, u32 size);
void init_leaf(tree_node **leaf, tree_node *parent);
void generate_subtrees(tree_node *parent);
void calculate_mean(tree_node *block, pixel **image, u32 size, u64 *red, u64 *green, u64 *blue, u64 *mean);
void init_vector(cell_array *vector);
void free_quadtree(tree_node *current);

pixel **read_ppm(FILE **in, char *name, u32 *size)
{
    if ((*in = fopen(name, "rb")) == NULL)
    {
        fprintf(stderr, "Error when opening image ppm file!");
    }
    char type[2];
    u32 max_value, i, j;
    fscanf(*in, "%s", type);
    fgetc(*in);
    fscanf(*in, "%u", size);
    fgetc(*in);
    fscanf(*in, "%u", size);
    fgetc(*in);
    fscanf(*in, "%u", &max_value);
    fgetc(*in);
    pixel **image = (pixel **)malloc(*size * sizeof(pixel *));

    for (i = 0; i < *size; ++i)
    {
        image[i] = (pixel *)malloc(*size * sizeof(pixel));
        for (j = 0; j < *size; ++j)
        {
            fread(&image[i][j].red, sizeof(char), 1, *in);
            fread(&image[i][j].green, sizeof(char), 1, *in);
            fread(&image[i][j].blue, sizeof(char), 1, *in);
        }
    }

    return image;
}

void init_tree(tree_node **root, u32 size)
{
    *root = (tree_node *)malloc(sizeof(tree_node));

    (*root)->type = 1;
    (*root)->corner_top_left.x = 0;
    (*root)->corner_top_left.y = 0;
    (*root)->corner_bottom_right.x = size - 1;
    (*root)->corner_bottom_right.y = size - 1;
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
    NW.x = parent->corner_top_left.x;
    NW.y = parent->corner_top_left.y;
    SE.x = parent->corner_bottom_right.x;
    SE.y = parent->corner_bottom_right.y;
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
    parent->top_left->corner_top_left.x = NW.x;
    parent->top_left->corner_top_left.y = NW.y;
    parent->top_left->corner_bottom_right.x = mid.x;
    parent->top_left->corner_bottom_right.y = mid.y;

    // Subarborele din dreapta sus:
    parent->top_right->corner_top_left.x = mid.x + 1;
    parent->top_right->corner_top_left.y = NW.y;
    parent->top_right->corner_bottom_right.x = SE.x;
    parent->top_right->corner_bottom_right.y = mid.y;

    // Subarborele din dreapta jos:
    parent->bottom_right->corner_top_left.x = mid.x + 1;
    parent->bottom_right->corner_top_left.y = mid.y + 1;
    parent->bottom_right->corner_bottom_right.x = SE.x;
    parent->bottom_right->corner_bottom_right.y = SE.y;

    // Subarborele din stanga jos:
    parent->bottom_left->corner_top_left.x = NW.x;
    parent->bottom_left->corner_top_left.y = mid.y + 1;
    parent->bottom_left->corner_bottom_right.x = mid.x;
    parent->bottom_left->corner_bottom_right.y = SE.y;
}

void calculate_mean(tree_node *current, pixel **image, u32 size, u64 *red, u64 *green, u64 *blue, u64 *mean)
{
    u32 i, j;

    for (i = current->corner_top_left.x; i <= current->corner_bottom_right.x; ++i)
    {
        for (j = current->corner_top_left.y; j <= current->corner_bottom_right.y; ++j)
        {
            *red += image[j][i].red;
            *green += image[j][i].green;
            *blue += image[j][i].blue;
        }
    }
    *red /= size * size;
    *green /= size * size;
    *blue /= size * size;

    for (i = current->corner_top_left.x; i <= current->corner_bottom_right.x; ++i)
    {
        for (j = current->corner_top_left.y; j <= current->corner_bottom_right.y; ++j)
        {
            *mean += (*red - image[j][i].red) * (*red - image[j][i].red) +
                     (*green - image[j][i].green) * (*green - image[j][i].green) +
                     (*blue - image[j][i].blue) * (*blue - image[j][i].blue);
        }
    }
    *mean /= 3 * size * size;
}

void init_vector(cell_array *vector)
{
    vector->array = (cell_data *)malloc(sizeof(cell_data));
    vector->length = 0;
    vector->capacity = 1;
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