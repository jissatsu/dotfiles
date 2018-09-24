#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

typedef char BYTE;
typedef unsigned int U_INT;

typedef struct {
	char title[50];
	char subject[50];
	char author[50];
	int book_id;

} BOOKS;

void printBook( BOOKS *book );

int main( void )
{
	BOOKS Book1;
	BOOKS Book2;

	strcpy( Book1.title, "W1at the fuck is this book1?" );
	strcpy( Book2.title, "W2at the fuck is this book2?" );

	printBook( &Book1 );
	printBook( &Book2 );

	return 0;	
}

void printBook( BOOKS *book )
{
	printf("Book title -> %p and value -> %c\n", (&book->title[0]), *(book->title + 0));	
	printf("Book title -> %p and value -> %c\n", (&book->title[1]), *(book->title + 1));	
}
