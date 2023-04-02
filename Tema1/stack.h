/* IUGA Darius-Gabriel-Ioan - 314CC */

#include <stdio.h>
#include <stdlib.h>

typedef struct stackNode stackNode;

// Un nod al stivelor UNDO/REDO
struct stackNode
{
    void *address;
    stackNode *next;
};

// Reprezentarea unei stive UNDO/REDO
typedef struct
{
    stackNode *top;
    stackNode *bottom;
} stack;

void initStack(stack *);
void push(stack *, stackNode *);
void* pop(stack *);
void flush(stack *);

void initStack(stack *insStack)
{
    insStack->top = NULL;
    insStack->bottom = NULL;
}

void push(stack *insStack, stackNode *node)
{
    stackNode *newNode = (stackNode *)malloc(sizeof(stackNode));
    newNode->address = node->address;
    if (insStack->top == NULL && insStack->bottom == NULL)
    {
        // Stiva este vida
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

// Returneaza adresa continuta de nodul din varf
void* pop(stack *insStack)
{
    stackNode *current = insStack->top;
    stackNode *remNode = (stackNode *)malloc(sizeof(stackNode));
    remNode->address = current->address;
    if (insStack->top == insStack->bottom)
    {
        // Stiva are un singur nod
        insStack->top = NULL;
        insStack->bottom = NULL;
        free(current);
    }
    else
    {
        insStack->top = insStack->top->next;
        free(current);
    }

    return remNode->address;
}

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
