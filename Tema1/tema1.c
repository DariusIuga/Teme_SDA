#include <stdio.h>
#include <stdlib.h>

typedef char T;
typedef struct node node;

struct node
{
    T data;
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
void show(doublyLinkedList*);
void showCurrent(doublyLinkedList*);

int main(void)
{
    doublyLinkedList band;
    initList(&band);
    addNode(&band, '#');

    printf("banda initial: ");
    show(&band);
    printf("banda: ");
    showCurrent(&band);
    printf("banda: ");
    show(&band);

    return 0;
}

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
        current=current->next;
    }
    printf("\n");
}

void showCurrent(doublyLinkedList *band)
{
    printf("%c\n", band->finger->data);
}
