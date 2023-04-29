#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_operations.h"

void statistics(tree_node *block, pixel **image, output_values *output, unsigned int factor, unsigned int size);
pixel **read_ppm(FILE *in, unsigned int *width, unsigned int *height);
void free_quadtree(tree_node *current);

int main(int argc, char **argv)
{
    unsigned int width, height, i;
    pixel **image;
    tree_node *root;
    output_values output;
    FILE *in, *out;

    if (argc == 5)
    {
        if (!strcmp(argv[1], "-c1"))
        {
            unsigned int factor = atoi(argv[2]);
            // if ((in = fopen("tests/input/test1.ppm", "rb")) == NULL)
            if ((in = fopen(argv[3], "rb")) == NULL)
            {
                fprintf(stderr, "Error when opening image ppm file!");
                return 1;
            }

            image = read_ppm(in, &width, &height);

            init_tree(&root, width, height);

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

            for (i = 0; i < height; ++i)
            {
                free(image[i]);
            }
            free(image);
            fclose(in);
            fclose(out);
        }
        else if (!strcmp(argv[1], "-c2"))
        {
            // unsigned int factor = atoi(argv[2]);
            // TODO
        }
    }
    else if (argc == 4 && !strcmp(argv[1], "-d"))
    {
        // TODO
    }
    else
    {
        printf("./quadtree [-c1 factor | -c2 factor | -d] [in] [out]\n");
    }

    free_quadtree(root);

    return 0;
}

void statistics(tree_node *block, pixel **image, output_values *output, unsigned int factor, unsigned int size)
{
    unsigned long long red = 0, green = 0, blue = 0, mean = 0;
    unsigned int i, j;

    for (i = block->corner_top_left.x; i <= block->corner_bottom_right.x; ++i)
    {
        for (j = block->corner_top_left.y; j <= block->corner_bottom_right.y; ++j)
        {
            red += image[i][j].red;
            green += image[i][j].green;
            blue += image[i][j].blue;
        }
    }
    red /= size * size;
    green /= size * size;
    blue /= size * size;

    for (i = block->corner_top_left.x; i <= block->corner_bottom_right.x; ++i)
    {
        for (j = block->corner_top_left.y; j <= block->corner_bottom_right.y; ++j)
        {
            mean += (red - image[i][j].red) * (red - image[i][j].red) +
                    (green - image[i][j].green) * (green - image[i][j].green) +
                    (blue - image[i][j].blue) * (blue - image[i][j].blue);
        }
    }
    mean /= 3 * size * size;

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

pixel **read_ppm(FILE *in, unsigned int *width, unsigned int *height)
{
    char type[2];
    unsigned int max_value, i, j;
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
