#include "cerinta1.h"
#include "cerinta2.h"
#include "cerinta3.h"

int main(int argc, char **argv)
{
    u32 size, i;
    pixel **image = NULL;
    tree_node *root = NULL;
    FILE *in = NULL, *out = NULL;

    if (argc == 5)
    {
        // Factorul de compresie
        u32 factor = atoi(argv[2]);
        // In cazul primelor doua cerinte, valorile RGB ale pixelilor si
        // marimea imaginilor sunt citie din fisierul ppm
        image = read_ppm(&in, argv[3], &size);
        init_tree(&root, size);

        if (!strcmp(argv[1], "-c1"))
        {
            // Cerinta 1

            // output retine valorile scrise in fisierul text
            output output;
            output.depth = 0;
            output.nr = 0;
            output.side = 0;

            // Analizam imaginea si arborele de compresie
            stats(root, image, &output, factor, size);
            // Scriem in fisierului aflat la path-ul dat sau cream unul nou
            write_text(&out, argv[4], output);
        }
        else if (!strcmp(argv[1], "-c2"))
        {
            // Cerinta 2

            // Reprezentarea sub forma liniara a arborelui,
            // care contine datele importante
            array vector;
            init_vector(&vector);
            // Transformam arborele de compresie in vector
            compression(root, image, factor, size, &vector);

            // Functia recursiva nu garanteaza ca nodurile de pe acelasi nivel
            // se vor afla alaturi in vector.
            // De aceea, vectorul trebuie sortat in functie de adancimea
            qsort(vector.array, vector.length, sizeof(data), cmp_depth);
            write_binary(&out, argv[4], vector, size);
            free(vector.array);
        }
    }
    else if (!strcmp(argv[1], "-d"))
    {
        // Cerinta 3

        array vector;
        init_vector(&vector);
        // Citim dimensiunea imaginii si vectorul din fisierul binar
        read_binary(&in, argv[2], &vector, &size);
        // Construim recursiv arborele cuaternar pe baza datelor din vector,
        // pornind de la indexul 0 al vectorului si nivelul 0 din arbore
        vector_to_tree(vector, &root, size, 0);

        // Alocarea memoriei pentru imagine
        image = (pixel **)malloc(size * sizeof(pixel *));
        for (i = 0; i < size; ++i)
        {
            image[i] = (pixel *)malloc(size * sizeof(pixel));
        }

        // Imaginea este generata recursiv prin colorarea zonelor delimitate
        // de nodurile frunza cu valorile RGB respective
        image = generate_image(image, root);
        // Datele reprezentate in matrice sunt scrise sub formatul ppm
        write_ppm(&out, argv[3], image, size);
        free(vector.array);
    }

    // Eliberarea memoriei si inchiderea fisierelor
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