#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "list.h"
#include "stack.h"

#define BUF_LENGTH 19

int main(void)
{
    doublyLinkedList band;
    instructionQueue queue;
    stack stack;
    FILE *input;
    char line[BUF_LENGTH], *token, operation, operand;
    unsigned short i, nrLinii;
    parameters instruction;

    initList(&band);
    moveRight(&band);
    initQueue(&queue);
    initStack(&stack);
    input = fopen("tema1.in", "r");
    if (input == NULL)
    {
        printf("Eroare la deschiderea fisierului tema1.in!\n");
        return 1;
    }

    fscanf(input, "%hu", &nrLinii);
    fgetc(input);
    for (i = 0; i < nrLinii; i++)
    {
        fgets(line, BUF_LENGTH, input);
        printf("%s\n", line);
        token = strtok(line, " \n");

        /*  Asocieri operatii de tip update - cod

        MOVE_LEFT == '1'
        MOVE_RIGHT == '2'
        MOVE_LEFT_CHAR == '3'
        MOVE_RIGHT_CHAR == '4'
        WRITE == '5'
        INSERT_LEFT == '6'
        INSERT_RIGHT == '7'

            Asocieri pentru stivele UNDO/REDO

        MOVE_LEFT='l'
        MOVE_RIGHT='r'

        */
        if (strcmp(token, "MOVE_LEFT") == 0)
        {
            enqueue(&queue, '1', '\0');
            push(&stack, 'l');
        }
        else if (strcmp(token, "MOVE_RIGHT") == 0)
        {
            enqueue(&queue, '2', '\0');
            push(&stack, 'r');
        }
        else if (strcmp(token, "MOVE_LEFT_CHAR") == 0)
        {
            token = strtok(NULL, " \n");
            operand = token[0];
            enqueue(&queue, '3', operand);
        }
        else if (strcmp(token, "MOVE_RIGHT_CHAR") == 0)
        {
            token = strtok(NULL, " \n");
            operand = token[0];
            enqueue(&queue, '4', operand);
        }
        else if (strcmp(token, "WRITE") == 0)
        {
            token = strtok(NULL, " \n");
            operand = token[0];
            enqueue(&queue, '5', operand);
            flush(&stack);
        }
        else if (strcmp(token, "INSERT_LEFT") == 0)
        {
            token = strtok(NULL, " \n");
            operand = token[0];
            enqueue(&queue, '6', operand);
        }
        else if (strcmp(token, "INSERT_RIGHT") == 0)
        {
            token = strtok(NULL, " \n");
            operand = token[0];
            enqueue(&queue, '7', operand);
        }
        else if (strcmp(token, "SHOW_CURRENT") == 0)
        {
            showCurrent(&band);
        }
        else if (strcmp(token, "SHOW") == 0)
        {
            show(&band);
        }
        else if (strcmp(token, "UNDO") == 0)
        {
            // operation = pop(stack);
        }
        else if (strcmp(token, "REDO") == 0)
        {
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
                moveLeft(&band);
                break;
            }
            case '2':
            {
                moveRight(&band);
                break;
            }
            case '3':
            {
                moveLeftChar(&band, operand);
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
                break;
            }
            case '6':
            {
                insertLeftChar(&band, operand);
                break;
            }
            case '7':
            {
                insertRightChar(&band, operand);
                break;
            }
            default:
            {
                fprintf(stderr, "Error: Unknown operation name\n");
                exit(1);
            }
            }
        }
    }

    fclose(input);

    // *** TESTE ***

    // *** TESTE ***

    return 0;
}
