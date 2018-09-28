#include <stdio.h>
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
#include <netinet/ip.h>
#include <endian.h>

typedef enum { 
		ALL_ZERO,
		INCREMENT 
} Actions;

void arrayClear( Actions action, int *array, size_t size );
void checkPrimeNumber( void );


int main( int argc, char *argv[] )
{
	checkPrimeNumber();
	return 0;	
}

void arrayClear( Actions action, int *array, size_t size )
{	
	switch( action )
	{
		case INCREMENT:
			for( int l = 0 ; l < size ; l++ ){
				array[l] = l;			
			}
		break;

		case ALL_ZERO:
			for( int l = 0 ; l < size ; l++ ){
				array[l] = 0 ;
			}
		break;
	}
	return;	
}

void checkPrimeNumber( void )
{
	int n, flag = 0, dvd = 2;
	int ints[10];
	size_t len = sizeof(ints) / sizeof(int);

	arrayClear( INCREMENT, ints, len );

	printf("Enter a positive integer -> ");
	scanf("%d", &n);

	if(n % dvd == 0){
		flag = 1;
	}

	if( flag == 1 ){
		printf("%d is not a prime number\n", n);
	} else {
		printf("%d is a prime number\n", n);
	}
}
