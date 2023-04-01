/* IUGA Darius-Gabriel-Ioan - 314CC */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "queue.h"
#include "stack.h"

#define BUF_LENGTH 19

int main(void)
{
    doublyLinkedList band;
    instructionQueue queue;
    stack undo, redo;
    parameters instruction;
    node *temp;
    stackNode *oldNode, *newNode;
    FILE *input, *output;

    char line[BUF_LENGTH], *token, operation, operand;
    unsigned short i, nrLinii;
    oldNode = (stackNode *)malloc(sizeof(stackNode));
    newNode = (stackNode *)malloc(sizeof(stackNode));

    initList(&band);
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
        fgets(line, BUF_LENGTH, input);
        printf("%s\n", line);
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
        else if (strcmp(token, "UNDO") == 0)
        {
            oldNode->address = pop(&undo);
            newNode->address = (void *)band.finger;
            push(&redo, newNode);
            moveFinger(&band, oldNode->address);
        }
        else if (strcmp(token, "REDO") == 0)
        {
            oldNode->address = pop(&redo);
            newNode->address = (void *)band.finger;
            push(&undo, newNode);
            moveFinger(&band, oldNode->address);
        }
        else if (strcmp(token, "EXECUTE") == 0)
        {
            instruction = dequeue(&queue);
            operation = instruction.opCode;
            operand = instruction.operand;

            switch (operation)
            {
            case '1':
            {
                temp = moveLeft(&band);
                if (temp != NULL)
                {
                    oldNode = (stackNode *)malloc(sizeof(stackNode));
                    oldNode->address = (void *)temp;
                    push(&undo, oldNode);
                }
                break;
            }
            case '2':
            {
                temp = moveRight(&band);
                oldNode = (stackNode *)malloc(sizeof(stackNode));
                oldNode->address = (void *)temp;
                push(&undo, oldNode);
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
                fprintf(output, "ERROR\n");
            }
            }
        }
        testShow(&band);
    }

    fclose(input);
    fclose(output);

    // *** TESTE ***

    // *** TESTE ***

    return 0;
}
