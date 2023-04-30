#include "cerinta2.h"
#include "cerinta1.h"

int main(int argc, char **argv)
{
    u32 width, height, i;
    pixel **image;
    tree_node *root;
    output_values output;
    FILE *in = NULL, *out = NULL;

    if (argc == 5)
    {
        u32 factor = atoi(argv[2]);
        image = read_ppm(&in, argv[3], &width, &height);
        init_tree(&root, width, height);

        if (!strcmp(argv[1], "-c1"))
        {
            // Cerinta 1
            output.depth = 0;
            output.nr = 0;
            output.side = 0;
            statistics(root, image, &output, factor, width);
            write_text(&out, argv[4], output);
        }
        else
        {
            // Cerinta 2
            cell_array vector;
            init_vector(&vector);
            compression(root, image, factor, width, vector);
            qsort(vector.array, vector.length, sizeof(cell_data), cmp_depth);
            write_binary(&out, argv[4], vector, width);
        }
        for (i = 0; i < height; ++i)
        {
            free(image[i]);
        }
        free(image);
    }
    else
    {
        // Cerinta 3
        // TODO
    }

    free_quadtree(root);
    fclose(in);
    fclose(out);

    return 0;
}