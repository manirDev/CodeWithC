#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 0x40

typedef struct bookType
{
    char title[BUFFER_SIZE];
    int pages;
    struct bookType *next;
} bookType;

bookType *first;
int numBook;

void addBook(char *title, int pages)
{
    bookType *p, *new;
    if (NULL == first)
    {
        p = malloc(sizeof(bookType));
        memset(p, 0, sizeof(bookType));
        numBook++;
        strncpy(p->title, title, BUFFER_SIZE - 1);
        p->pages = pages;
        p->next = NULL;
        first = p;
        return;
    }

    for (p = first; NULL != p->next; p = p->next)
        ;

    new = malloc(sizeof(bookType));
    memset(new, 0, sizeof(bookType));
    numBook++;
    strncpy(new->title, title, BUFFER_SIZE - 1);
    new->pages = pages;
    new->next = NULL;
    p->next = new;
}

void listBooks(char *inputStr)
{
    bookType *book;

    for (book = first; NULL != book; book = book->next)
    {
        if (NULL != inputStr || 0 == strcmp(inputStr, book->title))
        {
            printf("Pages: %d\tTitle: '%s'\n", book->pages, book->title);
        }
    }

}

int removeBook(char *inputStr)
{
    bookType *currBook, *nextBook;
    for (currBook = first; NULL != currBook; nextBook = currBook, currBook = currBook->next)
    {
        if (NULL != inputStr && 0 == strcmp(inputStr, currBook->title))
        {
            if (first == currBook)
            {
                first = currBook->next;
            }
            else
            {
                nextBook->next = (currBook->next) ? currBook->next : NULL;  
            }
            free(currBook);
            return 1;
        }
    }
    return 0;
}

int main()
{
    first = NULL;
    numBook = 0;

    addBook("Deep Work", 122);
    addBook("Atomic Habits", 343);
    addBook("Think and Grow Rich", 143);
    addBook("Clean Code", 677);
    addBook("Manir Koro Lang", 234);

    int removeRes = removeBook("Clean Code");
    printf("remove answ: %d\n", removeRes);
    listBooks("Manir Koro Lang");

    return 0;
}
