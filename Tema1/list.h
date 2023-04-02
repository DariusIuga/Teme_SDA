/* IUGA Darius-Gabriel-Ioan - 314CC */

#include <stdio.h>
#include <stdlib.h>

typedef struct node node;

// Un nod al benzii
struct node
{
    char data;
    node *prev;
    node *next;
};

// Reprezentarea intregii benzi
typedef struct
{
    node *head;
    node *tail;
    node *finger;
    node *sentinel;
} doublyLinkedList;

// Initializarea listei
void initList(doublyLinkedList *);

// Operatii de tip Update
void *moveLeft(doublyLinkedList *);
void *moveRight(doublyLinkedList *);
void moveLeftChar(FILE *, doublyLinkedList *, char);
void moveRightChar(doublyLinkedList *, char);
void writeChar(doublyLinkedList *, char);
void insertLeftChar(FILE *, doublyLinkedList *, char);
void insertRightChar(doublyLinkedList *, char);

// Operatii de tip Query
void showCurrent(FILE *file, doublyLinkedList *band);
void show(FILE *file, doublyLinkedList *band);

void moveFinger(doublyLinkedList *, void *);
void freeList(doublyLinkedList *);

void initList(doublyLinkedList *band)
{
    // Crearea nodului santinela
    band->sentinel = (node *)malloc(sizeof(node));
    band->sentinel->prev = NULL;
    band->sentinel->next = NULL;
    // Toti pointerii din structura benzii indica initial spre santinela
    band->head = band->sentinel;
    band->tail = band->sentinel;
    band->finger = band->sentinel;
}

void *moveLeft(doublyLinkedList *band)
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

void *moveRight(doublyLinkedList *band)
{
    node *temp = band->finger;
    // Se introduce un nod nou la finalul listei
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
            band->finger = current;
            return;
        }
        current = current->next;
    }

    // Daca se iese din while, nu s-a gasit niciun nod cu acest caracter,
    // deci se va introduce un nod nou la finalul listei, spre care va
    // pointa tail
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
        // Caracterul nu poate fi inserat in nodul santinela,
        // daca degetul pointeaza spre nodul din dreapta sa
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

// Muta degetul pe nodul de la adresa data, daca acesta se gaseste in lista
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

void freeList(doublyLinkedList *band)
{
    node *temp, *current = band->tail;
    while (current != band->sentinel)
    {
        temp = current->prev;
        free(current);
        current = temp;
    }

    free(band->sentinel);
    band->head = NULL;
    band->tail = NULL;
    band->finger = NULL;
    band->sentinel = NULL;
}
