#ifndef CERINTA2_H
#define CERINTA2_H

#include "tree_operations.h"

void init_vector(cell_array *vector);
void add_cell(cell_array *vector, u64 red, u64 green, u64 blue, tree_node *block);
void compression(tree_node *block, pixel **image, u32 factor, u32 size, cell_array *vector);
void write_binary(FILE **out, char *name, cell_array vector, u32 size);
int cmp_depth(const void *a, const void *b);

void init_vector(cell_array *vector)
{
    vector->array = (cell_data *)malloc(sizeof(cell_data));
    vector->length = 0;
    vector->capacity = 1;
}

void add_cell(cell_array *vector, u64 red, u64 green, u64 blue, tree_node *block)
{
    if (vector->length >= vector->capacity)
    {
        vector->capacity *= 2;
        vector->array = (cell_data *)realloc(vector->array, vector->capacity * sizeof(cell_data));
    }
    u32 i = vector->length;
    vector->array[i].type = block->type;

    // Daca tipul nodului curent e 1 (frunza),
    // stocam si valorile RGB
    if (vector->array[i].type)
    {
        vector->array[i].red = (unsigned char)red;
        vector->array[i].green = (unsigned char)green;
        vector->array[i].blue = (unsigned char)blue;
    }
    // Prin metoda reccursiva de parcurgere a vectorului,
    // elementele de pe acelasi nivel nu pot fi puse consecutiv in vector.
    // Acesta trebuie sortat crescator dupa adancimea nodurilor.
    vector->array[i].depth = block->depth;
    ++(vector->length);
}

void compression(tree_node *block, pixel **image, u32 factor, u32 size, cell_array *vector)
{
    u64 red = 0, green = 0, blue = 0, mean = 0;
    calculate_mean(block, image, size, &red, &green, &blue, &mean);

    if (mean > factor)
    {
        // Acest bloc va fi divizat in 4 sub-blocuri
        generate_subtrees(block);
        add_cell(vector, 0, 0, 0, block);
        size >>= 1;
        compression(block->top_left, image, factor, size, vector);
        compression(block->top_right, image, factor, size, vector);
        compression(block->bottom_right, image, factor, size, vector);
        compression(block->bottom_left, image, factor, size, vector);
    }
    else
    {
        add_cell(vector, red, green, blue, block);
    }
}

void write_binary(FILE **out, char *name, cell_array vector, u32 size)
{
    if ((*out = fopen(name, "wb")) == NULL)
    {
        fprintf(stderr, "Error when creating output file!");
    }
    u32 i;
    fwrite(&size, sizeof(u32), 1, *out);
    for (i = 0; i < vector.length; ++i)
    {
        fwrite(&(vector.array[i].type), sizeof(unsigned char), 1, *out);
        // Daca nodul e o frunza, scriem si valorile RGB
        if (vector.array[i].type)
        {
            fwrite(&(vector.array[i].red), sizeof(unsigned char), 1, *out);
            fwrite(&(vector.array[i].green), sizeof(unsigned char), 1, *out);
            fwrite(&(vector.array[i].blue), sizeof(unsigned char), 1, *out);
        }
    }
}

int cmp_depth(const void *a, const void *b)
{
    const cell_data *cell_a = (const cell_data *)a;
    const cell_data *cell_b = (const cell_data *)b;

    return cell_a->depth - cell_b->depth;
}

#endif