#include <stdio.h>
#include <stdlib.h>
#include "tree_operations.h"

void recursive_mean_color(tree_node *block, pixel **image, output_values *output, unsigned int factor, unsigned int size);
unsigned int square(unsigned int number);

int main(void)
{
    char type[2];
    unsigned int width, height, max_value, i, j, factor;
    pixel **image;
    tree_node *root;
    output_values output;
    FILE *ppm, *out;

    if ((ppm = fopen("tests/input/test0.ppm", "rb")) == NULL)
    {
        fprintf(stderr, "Error when opening image ppm file!");
        return 1;
    }

    fscanf(ppm, "%s", type);
    fgetc(ppm);
    fscanf(ppm, "%u", &width);
    fgetc(ppm);
    fscanf(ppm, "%u", &height);
    fgetc(ppm);
    fscanf(ppm, "%u", &max_value);
    fgetc(ppm);

    scanf("%u", &factor);

    image = (pixel **)malloc(height * sizeof(pixel *));
    for (i = 0; i < height; ++i)
    {
        image[i] = (pixel *)malloc(width * sizeof(pixel));
        for (j = 0; j < width; ++j)
        {
            fread(&image[i][j].red, sizeof(char), 1, ppm);
            fread(&image[i][j].green, sizeof(char), 1, ppm);
            fread(&image[i][j].blue, sizeof(char), 1, ppm);
        }
    }

    init_tree(&root, width, height);

    output.depth = 0;
    output.nr_final_blocks = 0;
    output.biggest_side = 0;
    recursive_mean_color(root, image, &output, factor, width);
    if ((out = fopen("quadtree.out", "wb")) == NULL)
    {
        fprintf(stderr, "Error when creating output file!");
        return 1;
    }
    fprintf(out, "%u\n", output.depth);
    fprintf(out, "%u\n", output.nr_final_blocks);
    fprintf(out, "%u", output.biggest_side);
    for (i = 0; i < height; ++i)
    {
        free(image[i]);
    }
    free(image);

    fclose(ppm);
    fclose(out);

    return 0;
}

void recursive_mean_color(tree_node *block, pixel **image, output_values *output, unsigned int factor, unsigned int size)
{
    unsigned long long red = 0, green = 0, blue = 0, mean = 0;
    unsigned int i=0, j=0;

    for (i = block->corner_top_left.x; i < block->corner_bottom_right.x; ++i)
    {
        for (j = block->corner_top_left.y; j < block->corner_bottom_right.y; ++j)
        {
            red += image[i][j].red;
            green += image[i][j].green;
            blue += image[i][j].blue;
        }
    }
    red /= square(size);
    green /= square(size);
    blue /= square(size);

    for (i = block->corner_top_left.x; i < block->corner_bottom_right.x; ++i)
    {
        for (j = block->corner_top_left.y; j < block->corner_bottom_right.y; ++j)
        {
            mean += square(red - image[i][j].red) + square(green - image[i][j].green) + square(blue - image[i][j].blue);
        }
    }
    mean /= 3 * square(size);

    if (output->depth < block->depth)
    {
        output->depth = block->depth;
    }
    if (mean > factor)
    {
        // Acest bloc va fi divizat in 4 sub-blocuri
        generate_subtrees(block);
        size >>= 1;
        recursive_mean_color(block->top_left, image, output, factor, size);
        recursive_mean_color(block->top_right, image, output, factor, size);
        recursive_mean_color(block->bottom_right, image, output, factor, size);
        recursive_mean_color(block->bottom_left, image, output, factor, size);
    }
    else
    {
        // Blocul nu va fi divizat in sub-blocuri
        if (output->biggest_side < size)
        {
            output->biggest_side = size;
        }
        ++(output->nr_final_blocks);
    }
}

unsigned int square(unsigned int number)
{
    return number * number;
}
