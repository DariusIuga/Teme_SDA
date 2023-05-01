#include "cerinta1.h"
#include "cerinta2.h"
#include "cerinta3.h"

int main(int argc, char **argv)
{
    u32 size, i;
    pixel **image = NULL;
    tree_node *root = NULL;
    FILE *in = NULL, *out = NULL;

    // DEBUG MODE //

    argc = 4;
    argv[1] = "-d";
    argv[2] = "/home/darius/Documents/VS/C_C++/SDA/Teme/Tema_2/tests/input/test0_c2.out";
    argv[3] = "quadtree.out";
    // argv[4]=

    if (argc == 5)
    {
        u32 factor = atoi(argv[2]);
        image = read_ppm(&in, argv[3], &size);
        init_tree(&root, size);

        if (!strcmp(argv[1], "-c1"))
        {
            // Cerinta 1
            output_values output;
            output.depth = 0;
            output.nr = 0;
            output.side = 0;
            statistics(root, image, &output, factor, size);
            write_text(&out, argv[4], output);
        }
        else if (!strcmp(argv[1], "-c2"))
        {
            // Cerinta 2
            cell_array vector;
            init_vector(&vector);
            compression(root, image, factor, size, &vector);
            qsort(vector.array, vector.length, sizeof(cell_data), cmp_depth);
            write_binary(&out, argv[4], vector, size);
            free(vector.array);
        }
    }
    else if (!strcmp(argv[1], "-d"))
    {
        // Cerinta 3
        cell_array vector;
        init_vector(&vector);
        read_binary(&in, argv[2], &vector, &size);
        // Alocarea memoriei pentru imagine
        image = (pixel **)malloc(size * sizeof(pixel *));
        for (i = 0; i < size; ++i)
        {
            image[i] = (pixel *)malloc(size * sizeof(pixel));
            if (image[i] == NULL)
            {
                printf("ERROR\n");
            }
        }
        vector_to_tree(vector, &root, size, 0, 0);
        image = generate_image(image, root);
        write_ppm(&out, argv[3], image, size);
        free(vector.array);
    }

    for (i = 0; i < size; ++i)
    {
        free(image[i]);
    }
    free(image);
    free_quadtree(root);
    fclose(in);
    fclose(out);

    return 0;
}