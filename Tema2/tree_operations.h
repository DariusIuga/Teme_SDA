#include <stdio.h>
#include <stdlib.h>

typedef struct pixel
{
    unsigned char red, green, blue;
} pixel;

typedef struct point
{
    unsigned int x, y;
} point;

typedef struct tree_node tree_node;
struct tree_node
{
    // 0==>intern; 1==>frunza;
    unsigned char type;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char depth;
    point corner_top_left;
    point corner_bottom_right;
    tree_node *top_left;
    tree_node *top_right;
    tree_node *bottom_right;
    tree_node *bottom_left;
};

typedef struct output_values
{
    unsigned int depth;
    unsigned int nr_final_blocks;
    unsigned int biggest_side;
} output_values;

void init_tree(tree_node **root, unsigned int width, unsigned int height)
{
    *root = (tree_node *)malloc(sizeof(tree_node));

    (*root)->type = 1;
    (*root)->corner_top_left.x = 0;
    (*root)->corner_top_left.y = 0;
    (*root)->corner_bottom_right.x = width - 1;
    (*root)->corner_bottom_right.y = height - 1;
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
    init_leaf(&parent->top_left,parent);
    init_leaf(&parent->top_right,parent);
    init_leaf(&parent->bottom_right,parent);
    init_leaf(&parent->bottom_left,parent);

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
