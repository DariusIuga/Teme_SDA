#ifndef CERINTA1_H
#define CERINTA1_H

#include "tree_operations.h"

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

void write_text(FILE **out, char *name, output_values output)
{
    if ((*out = fopen(name, "wt")) == NULL)
    {
        fprintf(stderr, "Error when creating output file!");
    }
    fprintf(*out, "%u\n%u\n%u\n", output.depth, output.nr, output.side);
}

#endif