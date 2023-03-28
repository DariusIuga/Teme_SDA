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
void addNode(doublyLinkedList *, char);
void show(doublyLinkedList *);
void showCurrent(doublyLinkedList *);
void moveLeft(doublyLinkedList *);
void moveRight(doublyLinkedList *);
void moveLeftChar(doublyLinkedList *, char);
void moveRightChar(doublyLinkedList *, char);
void writeChar(doublyLinkedList *, char);
void insertLeftChar(doublyLinkedList *, char);
void insertRightChar(doublyLinkedList *, char);

void initList(doublyLinkedList *band)
{
    node *sentinel = (node *)malloc(sizeof(node));
    sentinel->prev = NULL;
    sentinel->next = NULL;
    band->head = sentinel;
    band->tail = sentinel;
    band->finger = sentinel;
}

void addNode(doublyLinkedList *band, char elem)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->data = elem;
    band->head->next = newNode;
    band->tail = newNode;
    band->finger = newNode;
    newNode->prev = band->head;
    newNode->next = NULL;
}

void show(doublyLinkedList *band)
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

void showCurrent(doublyLinkedList *band)
{
    printf("%c\n", band->finger->data);
}

void moveLeft(doublyLinkedList *band)
{
    if (band->finger != band->head)
    {
        band->finger = band->finger->prev;
    }
}

void moveRight(doublyLinkedList *band)
{
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
}

void moveLeftChar(doublyLinkedList *band, char elem)
{
    node *current = band->finger;
    int gasit = 0;
    while (current != band->head)
    {
        if (current->data == elem)
        {
            band->finger = current;
            gasit = 1;
            break;
        }
        current = current->prev;
    }
    if (!gasit)
    {
        printf("ERROR\n");
    }
}

void moveRightChar(doublyLinkedList *band, char elem)
{
    node *current = band->finger;
    while (current != band->tail)
    {
        if (current->data == elem)
        {
            band->finger = current;
            break;
        }
        current = current->prev;
    }
    if (current->data != elem)
    {
        node *newNode = (node *)malloc(sizeof(node));
        band->tail->next = newNode;
        newNode->prev = band->tail;
        band->tail = newNode;
        band->tail->next = NULL;
        band->finger = band->tail;
    }
}

void writeChar(doublyLinkedList *band, char elem)
{
    band->finger->data = elem;
}

void insertLeftChar(doublyLinkedList *band, char elem)
{
    if (band->finger != band->head->next)
    {
        band->finger = band->finger->prev;
        band->finger->data = elem;
    }
    else
    {
        printf("ERROR\n");
    }
}

void insertRightChar(doublyLinkedList *band, char elem)
{
    if (band->finger == band->tail)
    {
        node *newNode = (node *)malloc(sizeof(node));
        newNode->data = elem;
        band->tail->next = newNode;
        newNode->prev = band->tail;
        band->tail = newNode;
        band->tail->next = NULL;
        band->finger = band->tail;
    }
    else
    {
        band->finger = band->finger->next;
        band->finger->data = elem;
    }
}