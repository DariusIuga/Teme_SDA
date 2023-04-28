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
    unsigned int depth;
    point corner_top_left;
    point corner_bottom_right;
    tree_node *top_left;
    tree_node *top_right;
    tree_node *bottom_right;
    tree_node *bottom_left;
};

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

void generate_subtrees(tree_node *parent)
{
    // North-West si South-East
    point NW, SE;
    NW.x = parent->corner_top_left.x;
    NW.y = parent->corner_top_left.y;
    SE.x = parent->corner_bottom_right.x;
    SE.y = parent->corner_bottom_right.y;

    // Nodul parinte devine unul de tip inner
    parent->type = 0;
    // Este alocata memorie pentru subarbori
    parent->top_left = (tree_node *)malloc(sizeof(tree_node));
    parent->top_right = (tree_node *)malloc(sizeof(tree_node));
    parent->bottom_right = (tree_node *)malloc(sizeof(tree_node));
    parent->bottom_left = (tree_node *)malloc(sizeof(tree_node));

    // Subarborele din stanga sus:
    parent->top_left->corner_top_left.x = NW.x;
    parent->top_left->corner_top_left.y = NW.y;
    parent->top_left->corner_bottom_right.x = (SE.x - NW.x) / 2;
    parent->top_left->corner_bottom_right.y = (SE.y - NW.y) / 2;

    // Subarborele din dreapta sus:
    parent->top_right->corner_top_left.x = (SE.x - NW.x) / 2 + 1;
    parent->top_right->corner_top_left.y = NW.y;
    parent->top_right->corner_bottom_right.x = SE.x;
    parent->top_right->corner_bottom_right.y = (SE.y - NW.y) / 2;

    // Subarborele din dreapta jos:
    parent->bottom_right->corner_top_left.x = (SE.x - NW.x) / 2 + 1;
    parent->bottom_right->corner_top_left.y = (SE.y - NW.y) / 2 + 1;
    parent->bottom_right->corner_bottom_right.x = SE.x;
    parent->bottom_right->corner_bottom_right.y = SE.y;

    // Subarborele din stanga jos:
    parent->bottom_left->corner_top_left.x = NW.x;
    parent->bottom_left->corner_top_left.y = (SE.y - NW.y) / 2 + 1;
    parent->bottom_left->corner_bottom_right.x = (SE.x - NW.x) / 2;
    parent->bottom_left->corner_bottom_right.y = SE.y;

    // Nivelul unui nod fiu e cu 1 mai mare fata de cel al parintelui sau
    parent->top_left->depth = (parent->depth) + 1;
    parent->top_right->depth = (parent->depth) + 1;
    parent->bottom_right->depth = (parent->depth) + 1;
    parent->bottom_left->depth = (parent->depth) + 1;

    // Subarborii noi vor fi frunze
    parent->top_left->type = 1;
    parent->top_right->type = 1;
    parent->bottom_right->type = 1;
    parent->bottom_left->type = 1;
}