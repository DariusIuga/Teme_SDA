#ifndef CERINTA2_H
#define CERINTA2_H

#include "tree_operations.h"

void add_cell(array *vector, u64 red, u64 green, u64 blue, tree_node *block);
void compression(tree_node *block, pixel **image,
                 u32 factor, u32 size, array *vector);
void write_binary(FILE **out, char *name, array vector, u32 size);
int cmp_depth(const void *a, const void *b);

void add_cell(array *vector, u64 red, u64 green, u64 blue, tree_node *block)
{
    if (vector->length >= vector->cap)
    {
        vector->cap *= 2;
        vector->array = (data *)
            realloc(vector->array, vector->cap * sizeof(data));
    }
    u32 i = vector->length;
    vector->array[i].type = block->type;

    // Daca tipul nodului curent e 1 (frunza),
    // retinem si valorile RGB
    if (vector->array[i].type)
    {
        vector->array[i].red = (u_char)red;
        vector->array[i].green = (u_char)green;
        vector->array[i].blue = (u_char)blue;
    }
    // Prin metoda reccursiva de parcurgere a vectorului,
    // elementele de pe acelasi nivel nu pot fi puse consecutiv in vector.
    // Acesta trebuie sortat crescator dupa adancimea nodurilor.
    vector->array[i].depth = block->depth;
    ++(vector->length);
}

void compression(tree_node *block, pixel **image,
                 u32 factor, u32 size, array *vector)
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

void write_binary(FILE **out, char *name, array vector, u32 size)
{
    if ((*out = fopen(name, "wb")) == NULL)
    {
        fprintf(stderr, "Error when writing to binary file!");
    }
    u32 i;
    fwrite(&size, sizeof(u32), 1, *out);
    for (i = 0; i < vector.length; ++i)
    {
        fwrite(&(vector.array[i].type), sizeof(u_char), 1, *out);
        // Daca nodul e o frunza, scriem si valorile RGB
        if (vector.array[i].type)
        {
            fwrite(&(vector.array[i].red), sizeof(u_char), 1, *out);
            fwrite(&(vector.array[i].green), sizeof(u_char), 1, *out);
            fwrite(&(vector.array[i].blue), sizeof(u_char), 1, *out);
        }
    }
}

int cmp_depth(const void *a, const void *b)
{
    const data *cell_a = (const data *)a;
    const data *cell_b = (const data *)b;

    return cell_a->depth - cell_b->depth;
}

#endif