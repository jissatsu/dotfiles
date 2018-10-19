#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <pcap.h>
#include <libnet.h>

#ifdef MAX_BUF_SIZE
	#undef MAX_BUF_SIZE
#endif
#define MAX_BUF_SIZE  1000

static int gettline( unsigned char * line );
static void fix_tabs( unsigned char * line );

int main( int argc, char *argv[] )
{
	unsigned char line[ MAX_BUF_SIZE ];
	size_t len = 0;

	while( (len = gettline( line )) > 0 )
	{
		fix_tabs( line );
	}
	return 0;
}

static int gettline( unsigned char * line )
{	
	int c;
	size_t len = 0;

	while( len < (MAX_BUF_SIZE - 1) && (c = getchar()) != EOF && c != '\n' )
	{
		*(line + (len++)) = c;
	}
	if( c == '\x0a' )
	{
		*(line + (len++)) = c;
	}
	*(line + len) = '\x00';

	return len;
}

static void fix_tabs( unsigned char * line )
{	
	int i;
	int	j = 0;
	int	save_pos = 0;

	unsigned char lline[ MAX_BUF_SIZE ];
	
	memset( lline, '\x00', sizeof(lline) / sizeof(unsigned char) );
	
	for( i = 0 ; line[i] != '\x00' ; i++ )
	{
		if( line[i] == '\x09' )
		{	
			if( save_pos == 0 ) { save_pos = i; }
			else { save_pos = j; }

			for( j = save_pos ; j < (save_pos + 4) ; j++ )
			{
				lline[j] = '\x20';
			}
		}
		else
		{
			lline[j] = line[i];
		}
		j++;
	}
	printf("%s\n", lline);	
}




