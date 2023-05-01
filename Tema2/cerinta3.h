#ifndef CERINTA3_H
#define CERINTA3_H

#include "tree_operations.h"

void read_binary(FILE **in, char *name, cell_array *vector, u32 *size);
void vector_to_tree(cell_array vector, tree_node **current, u32 size, u32 i, u32 depth);
pixel **generate_image(pixel **image, tree_node *current);
void write_ppm(FILE **out, char *name, pixel **image, u32 size);

void read_binary(FILE **in, char *name, cell_array *vector, u32 *size)
{
    if ((*in = fopen(name, "rb")) == NULL)
    {
        fprintf(stderr, "Error when reading from binary file!");
    }
    u32 i = vector->length;
    // nr_current_depth reprezinta nr de noduri de pe nivelul curent.
    // max_current_depth reprezinta nr total de noduri de pe un nivel,
    // frunze sau interne.
    u32 nr_current_depth = 0, max_current_depth = 1;
    unsigned char depth = 0;

    if (fread(size, sizeof(u32), 1, *in) != 1)
    {
        fprintf(stderr, "Error when reading size from binary file!");
        return;
    }

    while (fread(&(vector->array[i].type), sizeof(unsigned char), 1, *in) == 1)
    {
        // Cand s-au citit toate nodurile corespunzatoare unui nivel,
        // se trece la nivelul urmator si nr_current_depth se reseteaza.
        if (nr_current_depth >= max_current_depth)
        {
            ++depth;
            nr_current_depth = 0;
            max_current_depth *= 4;
        }
        ++nr_current_depth;
        vector->array[i].depth = depth;

        if (vector->length >= vector->capacity)
        {
            vector->capacity *= 2;
            vector->array = (cell_data *)realloc(vector->array, vector->capacity * sizeof(cell_data));
            if (vector->array == NULL)
            {
                fprintf(stderr, "Error when reallocating memory!");
                return;
            }
        }
        // Daca nodul e o frunza, citim si valorile RGB
        if (vector->array[i].type)
        {
            if (fread(&(vector->array[i].red), sizeof(unsigned char), 1, *in) != 1)
            {
                fprintf(stderr, "Error when reading red value from binary file!");
                return;
            }
            if (fread(&(vector->array[i].green), sizeof(unsigned char), 1, *in) != 1)
            {
                fprintf(stderr, "Error when reading green value from binary file!");
                return;
            }
            if (fread(&(vector->array[i].blue), sizeof(unsigned char), 1, *in) != 1)
            {
                fprintf(stderr, "Error when reading blue value from binary file!");
                return;
            }
        }
        ++(vector->length);
        i = vector->length;
    }
}

void vector_to_tree(cell_array vector, tree_node **current, u32 size, u32 i, u32 depth)
{
    if (*current == NULL)
    {
        init_tree(current, size);
    }
    (*current)->type = vector.array[i].type;
    (*current)->depth = depth;
    if ((*current)->type)
    {
        // Nodul e o frunza, retinem valorile RGB
        (*current)->red = vector.array[i].red;
        (*current)->green = vector.array[i].green;
        (*current)->blue = vector.array[i].blue;
    }
    else
    {
        generate_subtrees(*current);

        ++depth;
        i = i * 4 + 1;
        if (i + 3 >= vector.length)
        {
            // S-a ajuns la sfarsitul vectorului
            return;
        }
        (*current)->top_left->type = vector.array[i].type;
        (*current)->top_left->red = vector.array[i].red;
        (*current)->top_left->green = vector.array[i].green;
        (*current)->top_left->blue = vector.array[i].blue;

        (*current)->top_right->type = vector.array[i + 1].type;
        (*current)->top_right->red = vector.array[i + 1].red;
        (*current)->top_right->green = vector.array[i + 1].green;
        (*current)->top_right->blue = vector.array[i + 1].blue;

        (*current)->bottom_right->type = vector.array[i + 2].type;
        (*current)->bottom_right->red = vector.array[i + 2].red;
        (*current)->bottom_right->green = vector.array[i + 2].green;
        (*current)->bottom_right->blue = vector.array[i + 2].blue;

        (*current)->bottom_left->type = vector.array[i + 3].type;
        (*current)->bottom_left->red = vector.array[i + 3].red;
        (*current)->bottom_left->green = vector.array[i + 3].green;
        (*current)->bottom_left->blue = vector.array[i + 3].blue;

        vector_to_tree(vector, &((*current)->top_left), size / 2, i, depth);
        vector_to_tree(vector, &((*current)->top_right), size / 2, i + 1, depth);
        vector_to_tree(vector, &((*current)->bottom_right), size / 2, i + 2, depth);
        vector_to_tree(vector, &((*current)->bottom_left), size / 2, i + 3, depth);
    }
}

pixel **generate_image(pixel **image, tree_node *current)
{
    u32 i, j;
    if (current->type)
    {
        // Nodul curent e o frunza, coloram pixelii din zona asociata lui.
        for (i = current->corner_top_left.y; i <= current->corner_bottom_right.y; ++i)
        {
            for (j = current->corner_top_left.x; j <= current->corner_bottom_right.x; ++j)
            {
                image[i][j].red = current->red;
                image[i][j].green = current->green;
                image[i][j].blue = current->blue;
            }
        }
    }
    else
    {
        // Nodul curent e intern, trecem la fii lui
        generate_image(image, current->top_left);
        generate_image(image, current->top_right);
        generate_image(image, current->bottom_right);
        generate_image(image, current->bottom_left);
    }

    return image;
}

void write_ppm(FILE **out, char *name, pixel **image, u32 size)
{
    u32 i, j;
    if ((*out = fopen(name, "wb")) == NULL)
    {
        fprintf(stderr, "Error when writing to image ppm file!");
    }
    fprintf(*out, "%s", "P6\n");
    fprintf(*out, "%u %u\n", size, size);
    fprintf(*out, "%u\n", 255);

    for (i = 0; i < size; ++i)
    {
        for (j = 0; j < size; ++j)
        {
            fwrite(&image[i][j].red, sizeof(char), 1, *out);
            fwrite(&image[i][j].green, sizeof(char), 1, *out);
            fwrite(&image[i][j].blue, sizeof(char), 1, *out);
        }
    }
}

#endif