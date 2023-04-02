/* IUGA Darius-Gabriel-Ioan - 314CC */

#include <stdio.h>
#include <stdlib.h>

typedef struct queueNode queueNode;
typedef struct parameters parameters;

// Un nod al cozii pentru instructiun
struct queueNode
{
    char opCode;
    char operand;
    queueNode *next;
};

// Reprezentarea intregii cozi de instructiuni
typedef struct
{
    queueNode *front;
    queueNode *rear;
} instructionQueue;

// Parametrii folositi in main pentru executarea instructiunilor
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
    queueNode *newNode = (queueNode *)malloc(sizeof(queueNode));
    newNode->opCode = opCode;
    newNode->operand = operand;
    if (queue->front == NULL)
    {
        // Coada e vida
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
    queueNode *remNode;
    parameters opValues;

    if (queue->front == NULL)
    {
        fprintf(stderr, "Coada este vida\n");
        exit(1);
    }

    opValues.opCode = queue->front->opCode;
    opValues.operand = queue->front->operand;
    if (queue->front != queue->rear)
    {
        // Coada contine un singur nod
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

    return opValues;
}
