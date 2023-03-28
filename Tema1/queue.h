#include <stdio.h>
#include <stdlib.h>

typedef struct insNode insNode;

struct insNode
{
    char opCode;
    char operation;
    insNode *next;
};

typedef struct
{
    insNode *front;
    insNode *rear;
} instructionQueue;

void initQueue(instructionQueue *);
void enqueue(instructionQueue *, char, char);

void initQueue(instructionQueue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

void enqueue(instructionQueue *queue, char opCode, char operation)
{
    insNode *newNode = (insNode *)malloc(sizeof(newNode));
    newNode->opCode = opCode;
    newNode->operation = operation;
    if (queue->front == NULL)
    {
        queue->front = newNode;
        queue->rear = newNode;
        newNode->next = NULL;
    }
    else
    {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}