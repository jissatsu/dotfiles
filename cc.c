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


#define MAX_LEN   1000

static inline size_t gettline( char *line, int max );

int main( int argc, char **argv ) 
{	
	int c, except = 0, number = 0, r_code = 0;
	
	if( argc == 1 )
	{	
		r_code = -1;
		fprintf(stderr, "[ERROR] - Usage: %s -x -n pattern\n", basename(__FILE__));
		return ( r_code );
	}
	
	while( --argc && (*++argv)[0] == '-' )
	{	
		c = *++argv[0];
		switch( c )
		{
			case 'x':
				except = 1;
				break;
			case 'n':
				number = 1;
				break;
			default:
				argc = 0;
				r_code = -1;
				fprintf(stderr, "[ERROR] - Invalid option found!\n");
				break;
		}
	}
	return r_code;
}

static inline size_t gettline( char *line, int max )
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
