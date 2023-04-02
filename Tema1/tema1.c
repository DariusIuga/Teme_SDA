/* IUGA Darius-Gabriel-Ioan - 314CC */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "queue.h"
#include "stack.h"

#define BUF_LENGTH 19
typedef doublyLinkedList dLL;
typedef stackNode sNode;

void undoRedo(doublyLinkedList *, stackNode *, stackNode *, stack *, stack *);

int main(void)
{
    // Declararea structurilor folosite in program
    doublyLinkedList band;
    instructionQueue queue;
    stack undo, redo;
    parameters instruction;
    stackNode *oldNode, *newNode;
    FILE *input, *output;

    void *temp;
    char line[BUF_LENGTH], *token, operation, operand;
    unsigned short i, nrLinii;

    // Initializarea structurilor folosite
    initList(&band);
    // Pe langal nodul santinela, banda contine initial si un nod
    // cu un '#' drept continut
    moveRight(&band);
    initQueue(&queue);
    initStack(&undo);
    initStack(&redo);

    input = fopen("tema1.in", "r");
    output = fopen("tema1.out", "w");
    if (input == NULL)
    {
        fprintf(stderr, "Eroare la deschiderea fisierului tema1.in!\n");
        return 1;
    }
    if (output == NULL)
    {
        fprintf(stderr, "Eroare la deschiderea fisierului tema1.out!\n");
        return 1;
    }

    fscanf(input, "%hu", &nrLinii);
    fgetc(input);
    for (i = 0; i < nrLinii; i++)
    {
        // Citesc cate o linie in line
        fgets(line, BUF_LENGTH, input);
        // printf("%s\n", line);
        //  Initial token contine instructiunea citita pe linia curenta
        token = strtok(line, " \n");

        /*  Codificare operatii de tip update

        MOVE_LEFT == '1'
        MOVE_RIGHT == '2'
        MOVE_LEFT_CHAR == '3'
        MOVE_RIGHT_CHAR == '4'
        WRITE == '5'
        INSERT_LEFT == '6'
        INSERT_RIGHT == '7'

        */
        if (strcmp(token, "MOVE_LEFT") == 0)
        {
            enqueue(&queue, '1', '\0');
        }
        else if (strcmp(token, "MOVE_RIGHT") == 0)
        {
            enqueue(&queue, '2', '\0');
        }
        else if (strcmp(token, "MOVE_LEFT_CHAR") == 0)
        {
            // Pentru operatiile care au caracter drept parametru,
            // acesta e citit in token
            token = strtok(NULL, "\n");
            operand = token[0];
            enqueue(&queue, '3', operand);
        }
        else if (strcmp(token, "MOVE_RIGHT_CHAR") == 0)
        {
            token = strtok(NULL, "\n");
            operand = token[0];
            enqueue(&queue, '4', operand);
        }
        else if (strcmp(token, "WRITE") == 0)
        {
            token = strtok(NULL, "\n");
            operand = token[0];
            enqueue(&queue, '5', operand);
        }
        else if (strcmp(token, "INSERT_LEFT") == 0)
        {
            token = strtok(NULL, "\n");
            operand = token[0];
            enqueue(&queue, '6', operand);
        }
        else if (strcmp(token, "INSERT_RIGHT") == 0)
        {
            token = strtok(NULL, "\n");
            operand = token[0];
            enqueue(&queue, '7', operand);
        }
        else if (strcmp(token, "SHOW_CURRENT") == 0)
        {
            showCurrent(output, &band);
        }
        else if (strcmp(token, "SHOW") == 0)
        {
            show(output, &band);
        }
        // Apelez aceeasi functie pentru UNDO si REDO
        else if (strcmp(token, "UNDO") == 0)
        {

            oldNode = (stackNode *)malloc(sizeof(stackNode));
            newNode = (stackNode *)malloc(sizeof(stackNode));
            undoRedo(&band, oldNode, newNode, &undo, &redo);
            free(oldNode);
            free(newNode);
        }
        else if (strcmp(token, "REDO") == 0)
        {
            oldNode = (stackNode *)malloc(sizeof(stackNode));
            newNode = (stackNode *)malloc(sizeof(stackNode));
            undoRedo(&band, oldNode, newNode, &redo, &undo);
            free(oldNode);
            free(newNode);
        }
        else if (strcmp(token, "EXECUTE") == 0)
        {
            // Extrag codul si eventual parametrul
            // pentru operatia din fata cozii
            instruction = dequeue(&queue);
            operation = instruction.opCode;
            operand = instruction.operand;

            // In functie de codul primit execut operatia aferenta
            switch (operation)
            {
            case '1':
            {
                /*  Daca degetul este chiar la dreapta celulei santinela,
                    operatia MOVE_LEFT nu poate fi executata,
                    deci nu va fi introdusa adresa unui nod precedent
                    in stiva UNDO */
                temp = moveLeft(&band);
                if (temp != NULL)
                {
                    oldNode = (stackNode *)malloc(sizeof(stackNode));
                    oldNode->address = temp;
                    push(&undo, oldNode);
                    free(oldNode);
                }
                break;
            }
            case '2':
            {
                oldNode = (stackNode *)malloc(sizeof(stackNode));
                oldNode->address = moveRight(&band);
                push(&undo, oldNode);
                free(oldNode);
                break;
            }
            case '3':
            {
                moveLeftChar(output, &band, operand);
                break;
            }
            case '4':
            {
                moveRightChar(&band, operand);
                break;
            }
            case '5':
            {
                writeChar(&band, operand);
                // Golesc stivele la executia operatiei WRITE
                flush(&undo);
                flush(&redo);
                break;
            }
            case '6':
            {
                insertLeftChar(output, &band, operand);
                break;
            }
            case '7':
            {
                insertRightChar(&band, operand);
                break;
            }
            default:
            {
                // Sirul de caractere citit nu corespunde vreunei operatii
                fprintf(output, "ERROR\n");
            }
            }
        }
    }

    //  Se inchid fisierele folosite pentru input si output
    fclose(input);
    fclose(output);

    // Eliberarea memoriei folosite
    freeList(&band);
    flush(&undo);
    flush(&redo);

    return 0;
}

// Pentru UNDO stiva sursa e undo si stiva destinatie e redo
// Pentru REDO e invers
void undoRedo(dLL *band, sNode *old, sNode *new, stack *source, stack *dest)
{
    old->address = pop(source);
    new->address = (void *)band->finger;
    push(dest, new);
    moveFinger(band, old->address);
}
