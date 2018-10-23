#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <limits.h>
#include <time.h>
#include <libnet.h>
#include <pcap.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <linux/if_ether.h> 
#include <arpa/inet.h>
#include <errno.h>


#define MAX_LEN      1000
#define MAX_COUNT    1000
#define N_DEFAULT	   10

static size_t gettline( char *line, int max );
static void writelines( char **lines, int nlines, int arglines );

int main( int argc, char **argv ) 
{	
	size_t len = 0;
	int c, r_code = 0, lineno = 0;
	char line[ MAX_LEN ], *lines[ MAX_COUNT ];
	
	if( argc == 1 )
	{	
		r_code = -1;
		fprintf( stderr, "[ERROR] - Usage: %s -n N", basename(__FILE__) );
		return ( r_code );
	}

	if( (*++argv)[0] != '-' || *++argv[0] != 'n' )
	{	
		r_code = -1;
		fprintf( stderr, "[ERROR] - Usage: %s -n N\n", basename(__FILE__) );
		return r_code;
	}
	
	while( (len = gettline(line, MAX_LEN)) > 0 )
	{
		lines[ lineno ] = (char *) malloc( len );
		strcpy( lines[ lineno++ ], line );

		writelines( lines, (lineno - 1), 10 );
	}	
	
	return 0;
}

static size_t gettline( char *line, int max )
{
	int i = 0, c;

	while( i < (max - 1) && (c = getchar()) != EOF && c != '\x0a' )
	{
		*(line + (i++)) = c;
	}
	if( c == '\x0a' )
	{
		*(line + (i++)) = c;
	}
	*(line + (i++)) = '\x00';

	return (size_t) i;
}

static void writelines( char **lines, int nlines, int arglines )
{
	for( int i = nlines, j = 0 ; j < arglines && i >= 0 ; i--, j++ )
	{
		printf("%d: %s", j, lines[i]);
	}
}
