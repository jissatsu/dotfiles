#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pcap.h>
#include <unistd.h>
#include <libnet.h>
#include <pthread.h>
#include <netinet/ether.h>

#define MAX_LEN   1000

unsigned int is_alnum( unsigned char str[] );
size_t gettline( unsigned char str[] );
unsigned char line[ MAX_LEN ];

int main( int argc, char *argv[] )
{
	size_t len;
	while( ( len = gettline(line) ) != 0 )
	{
		if( !is_alnum( line ) )
		{
			printf("String either contains nums only or alphas only!\n");
		}	
		else
		{
			printf("%s\n", line);
		}
	}
	return 0;
}

unsigned int is_alnum( unsigned char str[] )
{	
	unsigned char alpha, num;
	alpha = num = 0;

	for( int i = 0 ; str[i] != '\0' ; i++ )
	{
		if( (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') )
		{
			alpha = 1;			
		}
		if( (str[i] >= '0' && str[i] <= '9') )
		{
			num = 1;
		}
	}

	if( !(alpha && num) ){ return 0; }
	else  { return 1; }
}

size_t gettline( unsigned char str[] )
{
	size_t i = 0;
	unsigned char c;

	while( (i < MAX_LEN) && (c = getchar()) != EOF && (c != '\n') )
	{
		line[ i ] = c;
		i++;	
	}	
	if( c == '\n' )
	{
		line[ i ] = '\0';
	}
	return i;
}
