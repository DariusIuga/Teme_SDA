#include <stdio.h>
#include <stdlib.h>

typedef struct stackNode stackNode;

struct stackNode
{
    char opCode;
    stackNode *next;
};

typedef struct
{
    stackNode *top;
    stackNode *bottom;
} stack;

void initStack(stack *);
void push(stack *, char);
//void pop(stack *, char);
void flush(stack *);

void initStack(stack *insStack)
{
    insStack->top = NULL;
    insStack->bottom = NULL;
}

void push(stack *insStack, char opCode)
{
    stackNode *newNode = (stackNode *)malloc(sizeof(stackNode));
    newNode->opCode = opCode;
    if (insStack->top == NULL || insStack->bottom == NULL)
    {
        insStack->top = newNode;
        insStack->bottom = newNode;
        insStack->top->next = NULL;
        insStack->bottom->next = NULL;
    }
    else
    {
        newNode->next = insStack->top;
        insStack->top = newNode;
    }
}

/*char pop(stack *insStack)
{
    stackNode *remNode = insStack->top;
    char opCode = remNode->opCode;
    if (insStack->top == insStack->bottom)
    {
        insStack->top = NULL;
        insStack->bottom = NULL;
        free(remNode);
    }
    else
    {
        insStack->top = insStack->top->next;
        free(remNode);
    }

    return opCode;
}*/

void flush(stack *insStack)
{
    stackNode *current = insStack->top;
    while (current != NULL)
    {
        stackNode *temp = current;
        current = current->next;
        free(temp);
    }

    insStack->top = NULL;
    insStack->bottom = NULL;
}