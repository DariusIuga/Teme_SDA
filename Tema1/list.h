/* IUGA Darius-Gabriel-Ioan - 314CC */

#include <stdio.h>
#include <stdlib.h>

typedef struct node node;

struct node
{
    char data;
    node *prev;
    node *next;
};

typedef struct
{
    node *head;
    node *tail;
    node *finger;
} doublyLinkedList;

void initList(doublyLinkedList *);

node *moveLeft(doublyLinkedList *);
node *moveRight(doublyLinkedList *);
void moveLeftChar(FILE *, doublyLinkedList *, char);
void moveRightChar(doublyLinkedList *, char);
void writeChar(doublyLinkedList *, char);
void insertLeftChar(FILE *, doublyLinkedList *, char);
void insertRightChar(doublyLinkedList *, char);
void showCurrent(FILE *file, doublyLinkedList *band);
void show(FILE *file, doublyLinkedList *band);

void moveFinger(doublyLinkedList *, void *);
void testShow(doublyLinkedList *);

void initList(doublyLinkedList *band)
{
    node *sentinel = (node *)malloc(sizeof(node));
    sentinel->prev = NULL;
    sentinel->next = NULL;
    band->head = sentinel;
    band->tail = sentinel;
    band->finger = sentinel;
}

node *moveLeft(doublyLinkedList *band)
{
    if (band->finger != band->head->next)
    {
        node *temp = band->finger;
        band->finger = band->finger->prev;
        return temp;
    }
    else
    {
        // Operatia nu s-a putut executa
        return NULL;
    }
}

node *moveRight(doublyLinkedList *band)
{
    node *temp = band->finger;
    if (band->finger == band->tail)
    {
        node *newNode = (node *)malloc(sizeof(node));
        newNode->data = '#';
        band->tail->next = newNode;
        newNode->prev = band->tail;
        band->tail = newNode;
        band->tail->next = NULL;
        band->finger = band->tail;
    }
    else
    {
        band->finger = band->finger->next;
    }

    return temp;
}

void moveLeftChar(FILE *file, doublyLinkedList *band, char elem)
{
    node *current = band->finger;
    int gasit = 0;
    while (current != band->head)
    {
        if (current->data == elem)
        {
            band->finger = current;
            gasit = 1;
            return;
        }
        current = current->prev;
    }
    if (!gasit)
    {
        fprintf(file, "ERROR\n");
    }
}

void moveRightChar(doublyLinkedList *band, char elem)
{
    node *current = band->finger;
    while (current != band->tail->next)
    {
        if (current->data == elem)
        {
            band->finger->prev = current->prev;
            band->finger->next = current->next;
            band->finger = current;
            return;
        }
        current = current->next;
    }

    node *newNode = (node *)malloc(sizeof(node));
    newNode->data = '#';
    band->tail->next = newNode;
    newNode->prev = band->tail;
    band->tail = newNode;
    band->tail->next = NULL;
    band->finger = band->tail;
}

void writeChar(doublyLinkedList *band, char elem)
{
    band->finger->data = elem;
}

void insertLeftChar(FILE *file, doublyLinkedList *band, char elem)
{
    if (band->finger != band->head->next)
    {
        node *newNode = (node *)malloc(sizeof(node));
        newNode->data = elem;
        newNode->next = band->finger;
        newNode->prev = band->finger->prev;
        band->finger->prev->next = newNode;
        band->finger->prev = newNode;
        band->finger = newNode;
    }
    else
    {
        fprintf(file, "ERROR\n");
    }
}

void insertRightChar(doublyLinkedList *band, char elem)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->data = elem;
    if (band->finger == band->tail)
    {
        band->tail->next = newNode;
        newNode->prev = band->tail;
        band->tail = newNode;
        band->tail->next = NULL;
        band->finger = band->tail;
    }
    else
    {
        newNode->next = band->finger->next;
        newNode->prev = band->finger;
        band->finger->next->prev = newNode;
        band->finger->next = newNode;
        band->finger = band->finger->next;
    }
}

void showCurrent(FILE *file, doublyLinkedList *band)
{
    fprintf(file, "%c\n", band->finger->data);
}

void show(FILE *file, doublyLinkedList *band)
{
    node *current = band->head->next;
    while (current != NULL)
    {
        if (current == band->finger)
        {
            fprintf(file, "|%c|", current->data);
        }
        else
        {
            fprintf(file, "%c", current->data);
        }
        current = current->next;
    }
    fprintf(file, "\n");
}

void testShow(doublyLinkedList *band)
{
    node *current = band->head->next;
    while (current != NULL)
    {
        if (current == band->finger)
        {
            printf("|%c|", current->data);
        }
        else
        {
            printf("%c", current->data);
        }
        current = current->next;
    }
    printf("\n");
}

void moveFinger(doublyLinkedList *band, void *address)
{
    node *current = band->head->next;
    while (current != NULL)
    {
        if ((void *)current == address)
        {
            band->finger = current;
            return;
        }
        else
        {
            current = current->next;
        }
    }
    fprintf(stderr, "Nodul cu aceasta adresa nu se gaseste in lista!\n");
}
