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
#define SPACE 	   '\x20'
#define TAB            4

char line[ MAX_LINE ];
char detabbed[ MAX_LINE ];

int gettline( void );
int copy( void );
void detab_string( char string[], char detabbed_string[] );


int main( int argc, char *argv[] )
{	
	size_t len;

	while( (len = gettline()) > 0 )
	{
		detab_string( line, detabbed );
		printf("%s\n", detabbed);

		strcpy( detabbed, "" );
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
		line[i] = c; i++;
	}
	if( c == '\n' )
	{
		line[i] = '\x00';
	}
	return i;
}

void detab_string( char string[], char detabbed_string[] )
{
	int i = 0, pos = 0 ;
	size_t length = strlen( string );	

	while( i < length )
	{	
		if( string[i] == '\x09' )
		{	
			pos = i;
			length += 4;	
			for( int j = i ; j < ( TAB + i ) ; j++ )
			{
				detabbed_string[ j ] = SPACE;						
			}
			i += 3;
		}
		else
		{
			if( pos != 0x00 ){
				pos++;
				detabbed_string[ i ] = string[ pos ];	
			}
			else{
				detabbed_string[ i ] = string[ i ];
			}
		}
		i++;
	}			
}
