#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
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

#define XMAX  50
#define YMAX  50
#define min( a, b ) ( (a) < (b) ? (a) : (b) )
#define max( a, b ) ( (a) > (b) ? (a) : (b) )

struct point { int x; int y; };

struct shape 
{
	int sides;
	int verticies;
	struct point point1;
	struct point point2;
	struct point middlePoint;
};

struct point makepoint( int x, int y );

int main( int argc, char **argv ) 
{	
	struct shape recta, trng, *rect = &recta;

	rect->sides       = 4;
	rect->verticies   = 4;
	rect->point1	  = makepoint( 0, 0 );
	rect->point2 	  = makepoint( XMAX, YMAX );

	rect->middlePoint = makepoint( (rect->point1.x + rect->point2.x) / 2,
				   				   (rect->point1.y + rect->point2.y) / 2 );

	fprintf( stdout, "Rectangle's sides     -> %d\n",      (*rect).sides );
	fprintf( stdout, "Rectangle's verticies -> %d\n\n",    (*rect).verticies );

	fprintf( stdout, "Rectangle's point 1 x -> %d\n",      rect->point1.x );
	fprintf( stdout, "Rectangle's point 1 y -> %d\n\n",    rect->point1.y );

	fprintf( stdout, "Rectangle's point 2 x -> %d\n",      rect->point2.x );
	fprintf( stdout, "Rectangle's point 2 y -> %d\n\n",    rect->point2.y );

	fprintf( stdout, "Rectangle's middle point -> %d\n\n", (rect->middlePoint.x + rect->middlePoint.y) );

	fprintf( stdout, "makepoint x -> %d\n", makepoint( 0, 1 ).x );
	fprintf( stdout, "makepoint y -> %d\n", makepoint( 0, 1 ).y );
	
	return 0;
}

struct point makepoint( int x, int y )
{
	struct point t;

	t.x = x;
	t.y = y;
	return t;
}
