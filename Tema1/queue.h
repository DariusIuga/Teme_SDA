#include <stdio.h>
#include <stdlib.h>

typedef struct insNode insNode;
typedef struct parameters parameters;

struct insNode
{
    char opCode;
    char operand;
    insNode *next;
};

typedef struct
{
    insNode *front;
    insNode *rear;
} instructionQueue;

struct parameters
{
    char opCode;
    char operand;
};

void initQueue(instructionQueue *);
void enqueue(instructionQueue *, char, char);
parameters dequeue(instructionQueue *);

void initQueue(instructionQueue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

void enqueue(instructionQueue *queue, char opCode, char operand)
{
    insNode *newNode = (insNode *)malloc(sizeof(insNode));
    newNode->opCode = opCode;
    newNode->operand = operand;
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

parameters dequeue(instructionQueue *queue)
{
    insNode *remNode;
    parameters x;

    if (queue->front == NULL)
    {
        printf("Coada este vida\n");
        exit(1);
    }

    x.opCode = queue->front->opCode;
    x.operand = queue->front->operand;
    if (queue->front != queue->rear)
    {
        remNode = queue->front;
        queue->front = queue->front->next;
        free(remNode);
    }
    else
    {
        free(queue->front);
        queue->front = NULL;
        queue->rear = NULL;
    }

    return x;
}
