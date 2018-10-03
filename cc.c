#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pcap.h>
#include <unistd.h>
#include <libnet.h>
#include <pthread.h>
#include <netinet/ether.h>

#define MAX_LINE	1000
#define SPACE 		   1
#define TAB   (SPACE * 4)

char line[ MAX_LINE ];

int gettline( void );
int copy( void );


int main( int argc, char *argv[] )
{	
	size_t len;

	while( (len = gettline()) > 0 )
	{
		printf("%s\n", line);
		strcpy( line, "" );
	}	
	return 0;
}

int gettline( void )
{
	int c, i = 0;
	extern char line[];

	while( (i < MAX_LINE) && (c = getchar()) != EOF && c != '\x0a' )
	{
		if( c == '\x09' )
		{
			for( int j = i ; j < (i + TAB) ; j++ )
			{
				line[j] = '\x20';
			}
			i += 3;
		}		
		else
		{
			line[i] = c;
		}
		i++;
	}
	if( c == '\x0a' )
	{
		i++; line[i] = '\x00';
	}
	return i;
}
