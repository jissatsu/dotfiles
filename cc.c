#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <regex.h>
#include <pcap.h>
#include <signal.h>
#include <unistd.h>
#include <libnet.h>
#include <netinet/in.h>
#include <endian.h>
#include "banner.h"

extern int errno;

typedef enum 
{
	MULTIPLICATION,
	DIVISION,
	SUBTRACTION,
	ADDITION
} 
OPS;

int scanOperation( void );
int checkGreaterThanZero( int n1, int n2 );


int main( int argc, char *argv[] )
{	
	displayHelp();
	int operation = scanOperation();
	int n1, n2;
	
	printf("You can choose only 2 numbers\nFirst -> ");
	scanf( "%d", &n1 );

	printf("Second -> ");
	scanf( "%d", &n2 );

	switch( operation )
	{
		case MULTIPLICATION:
			if( checkGreaterThanZero( n1, n2 ) != 0 )
			{
				fprintf(stderr, "[ERROR] -> Cant use zero in multiplication!\n");	
				exit( -1 );
			}
			printf( "%d * %d = %.2lf\n", n1, n2, (double) n1 * n2 );
		break;

		case DIVISION:
			if( checkGreaterThanZero( n1, n2 ) != 0 )
			{
				fprintf(stderr, "[ERROR] -> Cant use zero in division!\n");	
				exit( -1 );
			}
			printf( "%d / %d = %.2lf\n", n1, n2, (double) n1 / n2 );
		break;

		case SUBTRACTION:
			printf( "%d - %d = %.2lf\n", n1, n2, (double) n1 - n2 );
		break;

		case ADDITION:
			printf( "%d + %d = %.2lf\n", n1, n2, (double) n1 + n2 );
		break;

	}

	return 0;
}

int scanOperation( void )
{
	int op;

	printf("\nChoose your operation -> ");
	scanf("%d", &op);	
	
	if( (op < 0) || (op > 3) )
	{
		fprintf(stderr, "[ERROR] -> OP number invalid!\n");
		exit( EXIT_FAILURE );
	}

	return op;
}

int checkGreaterThanZero( int n1, int n2 )
{
	if( (n1 == 0) || (n2 == 0) )
	{
		return EXIT_FAILURE;
	}	
	return EXIT_SUCCESS;
}
