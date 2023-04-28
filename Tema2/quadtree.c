#include <stdio.h>
#include <stdlib.h>
#include "tree_operations.h"

void recursive_mean_color(tree_node *block);

int main(void)
{
    char type[2];
    unsigned int width, height, max_value, i, j;
    unsigned long long red = 0, green = 0, blue = 0, mean = 0;
    pixel **image;
    tree_node *root;
    FILE *ppm;
    if ((ppm = fopen("tests/input/test0.ppm", "rb")) == NULL)
    {
        fprintf(stderr, "Error when opening file!");
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

    init_tree(&root, width, height);
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

    generate_subtrees(root);
    for (i = 0; i < height; ++i)
    {
        free(image[i]);
    }
    free(image);

    return 0;
}

void recursive_mean_color(tree_node *block)
{
    // TODO
}
