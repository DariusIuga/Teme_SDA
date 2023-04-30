#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_operations.h"

pixel **read_ppm(FILE *in, u32 *width, u32 *height);
void calculate_mean(tree_node *block, pixel **image, u32 size, u64 *red, u64 *green, u64 *blue, u64 *mean);
void statistics(tree_node *block, pixel **image, output_values *output, u32 factor, u32 size);
void compression(tree_node *block, pixel **image, u32 factor, u32 size, cell_array vector);
void write_binary(FILE *out, cell_array vector, u32 size);
void free_quadtree(tree_node *current);

int main(int argc, char **argv)
{
    u32 width, height, i;
    pixel **image;
    tree_node *root;
    output_values output;
    FILE *in, *out;

    if (argc == 5)
    {
        u32 factor = atoi(argv[2]);
        // if ((in = fopen("tests/input/test1.ppm", "rb")) == NULL)
        if ((in = fopen(argv[3], "rb")) == NULL)
        {
            fprintf(stderr, "Error when opening image ppm file!");
            return 1;
        }

        image = read_ppm(in, &width, &height);

        init_tree(&root, width, height);

        if (!strcmp(argv[1], "-c1"))
        {
            // Cerinta 1
            output.depth = 0;
            output.nr = 0;
            output.side = 0;
            statistics(root, image, &output, factor, width);
            if ((out = fopen(argv[4], "wt")) == NULL)
            {
                fprintf(stderr, "Error when creating output file!");
                return 1;
            }
            fprintf(out, "%u\n%u\n%u\n", output.depth, output.nr, output.side);
        }
        else
        {
            // Cerinta 2
            cell_array vector;
            vector.array = (cell_data *)malloc(sizeof(cell_data));
            vector.length = 0;
            vector.capacity = 1;
            compression(root, image, factor, width, vector);
            qsort(vector.array, vector.length, sizeof(cell_data), cmp_depth);

            if ((out = fopen(argv[4], "wb")) == NULL)
            {
                fprintf(stderr, "Error when creating output file!");
                return 1;
            }
            write_binary(out, vector, width);
        }
        for (i = 0; i < height; ++i)
        {
            free(image[i]);
        }
        free(image);
        fclose(in);
        fclose(out);
    }
    else
    {
        // Cerinta 3
        // TODO
    }

    free_quadtree(root);

    return 0;
}

pixel **read_ppm(FILE *in, u32 *width, u32 *height)
{
    char type[2];
    u32 max_value, i, j;
    pixel **image;
    fscanf(in, "%s", type);
    fgetc(in);
    fscanf(in, "%u", width);
    fgetc(in);
    fscanf(in, "%u", height);
    fgetc(in);
    fscanf(in, "%u", &max_value);
    fgetc(in);

    image = (pixel **)malloc(*height * sizeof(pixel *));
    for (i = 0; i < *height; ++i)
    {
        image[i] = (pixel *)malloc(*width * sizeof(pixel));
        for (j = 0; j < *width; ++j)
        {
            fread(&image[i][j].red, sizeof(char), 1, in);
            fread(&image[i][j].green, sizeof(char), 1, in);
            fread(&image[i][j].blue, sizeof(char), 1, in);
        }
    }

    return image;
}

void calculate_mean(tree_node *block, pixel **image, u32 size, u64 *red, u64 *green, u64 *blue, u64 *mean)
{
    u32 i, j;

    for (i = block->corner_top_left.x; i <= block->corner_bottom_right.x; ++i)
    {
        for (j = block->corner_top_left.y; j <= block->corner_bottom_right.y; ++j)
        {
            *red += image[i][j].red;
            *green += image[i][j].green;
            *blue += image[i][j].blue;
        }
    }
    *red /= size * size;
    *green /= size * size;
    *blue /= size * size;

    for (i = block->corner_top_left.x; i <= block->corner_bottom_right.x; ++i)
    {
        for (j = block->corner_top_left.y; j <= block->corner_bottom_right.y; ++j)
        {
            *mean += (*red - image[i][j].red) * (*red - image[i][j].red) +
                     (*green - image[i][j].green) * (*green - image[i][j].green) +
                     (*blue - image[i][j].blue) * (*blue - image[i][j].blue);
        }
    }
    *mean /= 3 * size * size;
}

void statistics(tree_node *block, pixel **image, output_values *output, u32 factor, u32 size)
{
    u64 red = 0, green = 0, blue = 0, mean = 0;
    calculate_mean(block, image, size, &red, &green, &blue, &mean);

    if (output->depth <= block->depth)
    {
        output->depth = (block->depth) + 1;
    }
    if (mean > factor)
    {
        // Acest bloc va fi divizat in 4 sub-blocuri
        generate_subtrees(block);
        size >>= 1;
        statistics(block->top_left, image, output, factor, size);
        statistics(block->top_right, image, output, factor, size);
        statistics(block->bottom_right, image, output, factor, size);
        statistics(block->bottom_left, image, output, factor, size);
    }
    else
    {
        // Blocul nu va fi divizat in sub-blocuri
        if (output->side < size)
        {
            output->side = size;
        }
        ++(output->nr);
    }
}

void compression(tree_node *block, pixel **image, u32 factor, u32 size, cell_array vector)
{
    u64 red = 0, green = 0, blue = 0, mean = 0;
    calculate_mean(block, image, size, &red, &green, &blue, &mean);
    // TODO: Store the cells in the array

    if (vector.length >= vector.capacity)
    {
        vector.capacity *= 2;
        vector.array = (cell_data *)realloc(vector.array, vector.capacity * sizeof(cell_data));
    }
    u32 i = vector.length;
    vector.array[i].type = block->type;

    // Daca tipul nodului curent e 1 (frunza),
    // stocam si valorile RGB
    if (vector.array[i].type)
    {
        vector.array[i].red = (unsigned char)red;
        vector.array[i].green = (unsigned char)green;
        vector.array[i].blue = (unsigned char)blue;
    }
    // Prin metoda reccursiva de parcurgere a vectorului,
    // elementele de pe acelasi nivel nu pot fi puse consecutiv in vector.
    // Acesta trebuie sortat crescator dupa adancimea nodurilor.
    vector.array[i].depth = block->depth;
    ++(vector.length);

    if (mean > factor)
    {
        // Acest bloc va fi divizat in 4 sub-blocuri
        generate_subtrees(block);
        size >>= 1;
        compression(block->top_left, image, factor, size, vector);
        compression(block->top_right, image, factor, size, vector);
        compression(block->bottom_right, image, factor, size, vector);
        compression(block->bottom_left, image, factor, size, vector);
    }
}

void write_binary(FILE *out, cell_array vector, u32 size)
{
    u32 i;
    fwrite(&size, sizeof(u32), 1, out);
    for (i = 0; i < vector.length; ++i)
    {
        fwrite(&(vector.array[i].type), sizeof(unsigned char), 1, out);
        // Daca nodul e o frunza, scriem si valorile RGB
        if (vector.array[i].type)
        {
            fwrite(&(vector.array[i].red), sizeof(unsigned char), 1, out);
            fwrite(&(vector.array[i].green), sizeof(unsigned char), 1, out);
            fwrite(&(vector.array[i].blue), sizeof(unsigned char), 1, out);
        }
    }
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
