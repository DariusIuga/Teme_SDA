#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "list.h"

#define BUF_LENGTH 100

int main(void)
{
    doublyLinkedList band;
    FILE *input;
    char line[BUF_LENGTH], *token;
    int i, nrLinii;

    initList(&band);
    addNode(&band, '#');

    input = fopen("tema1.in", "r");
    if (input == NULL)
    {
        printf("Eroare la deschiderea fisierului tema1.in!\n");
        return 1;
    }

    fscanf(input, "%d", &nrLinii);
    fgetc(input);
    for (i = 0; i < nrLinii; i++)
    {
        fgets(line, BUF_LENGTH, input);
        printf("%s", line);
        token = strtok(line, " \n");
        if (strcmp(token, "MOVE_LEFT") == 0)
        {
            printf("Instructiunea MOVE_LEFT\n");
        }
        else if (strcmp(token, "MOVE_RIGHT") == 0)
        {
            printf("Instructiunea MOVE_RIGHT\n");
        }
        else if (strcmp(token, "MOVE_LEFT_CHAR") == 0)
        {
            printf("Instructiunea MOVE_LEFT_CHAR\n");
            printf("Caracterul e %s\n", token);
        }
        else if (strcmp(token, "MOVE_RIGHT_CHAR") == 0)
        {
            printf("Instructiunea MOVE_LEFT\n");
            token = strtok(NULL, " \n");
            printf("Caracterul e %s\n", token);
        }
        else if (strcmp(token, "WRITE") == 0)
        {
            printf("Instructiunea WRITE\n");
            token = strtok(NULL, " \n");
            printf("Caracterul e %s\n", token);
        }
        else if (strcmp(token, "INSERT_LEFT") == 0)
        {
            printf("Instructiunea INSERT_LEFT\n");
            token = strtok(NULL, " \n");
            printf("Caracterul e %s\n", token);
        }
        else if (strcmp(token, "INSERT_RIGHT") == 0)
        {
            printf("Instructiunea INSERT_RIGHT\n");
            token = strtok(NULL, " \n");
            printf("Caracterul e %s\n", token);
        }
        else if (strcmp(token, "SHOW_CURRENT") == 0)
        {
            printf("Instructiunea SHOW_CURRENT\n");
            token = strtok(NULL, " \n");
            printf("Caracterul e %s\n", token);
        }
        else if (strcmp(token, "SHOW") == 0)
        {
            printf("Instructiunea SHOW\n");
        }
        else if (strcmp(token, "UNDO") == 0)
        {
            printf("Instructiunea UNDO\n");
        }
        else if (strcmp(token, "REDO") == 0)
        {
            printf("Instructiunea REDO\n");
        }
        else if (strcmp(token, "EXECUTE") == 0)
        {
            printf("Instructiunea EXECUTE\n");
        }
    }

    // *** TESTE ***

    printf("banda initial: ");
    show(&band);
    printf("banda: ");
    moveRight(&band);
    show(&band);
    printf("banda: ");
    writeChar(&band, 'X');
    show(&band);
    printf("banda: ");
    moveRight(&band);
    show(&band);
    printf("banda: ");
    writeChar(&band, 'Y');
    show(&band);
    printf("banda: ");
    moveLeft(&band);
    show(&band);
    printf("banda: ");
    moveRight(&band);
    show(&band);

    // *** TESTE ***

    return 0;
}

