#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>
#include <curl/curl.h>

unsigned char * dec2hex( int num );
unsigned char * strrev( unsigned char const *str );
unsigned char * urlencode( unsigned char const *str );
void freeReversedString( unsigned char *str );

int main( void ) {
	unsigned char *hex;
	
	hex = dec2hex( 201 );
	fprintf( stdout, "%s\n", hex );
	freeReversedString( hex );
	
	return 0;
}

unsigned char * urlencode( unsigned char const *str ) {
	int i;
	size_t len;
	static unsigned char *encoded;
	
	len = strlen( str );
	return 0;
}

unsigned char * dec2hex( int num ) {
	int i = 0;
	int dec, remainder = 0;
	static unsigned char hex_str[0xFF];
	
	dec = num;
	while ( dec ) {
		remainder = dec % 16;
		if ( remainder <= 9 && remainder >= 0 ) {
			hex_str[i] = remainder + '0';
		} else{
			hex_str[i] = remainder + 55;
		}
		dec = dec / 16;
		++i;
	}
	hex_str[i] = '\x00';
	return strrev( hex_str );
}

unsigned char * strrev( unsigned char const *str ) {
	int i, j;
	size_t len;
	unsigned char *newStr;
	
	len = strlen( str );
	newStr = (unsigned char *) malloc( len * sizeof( char ) );

	for ( i = (len - 1), j = 0 ; i >= 0 ; i--, j++ ) {
		newStr[j] = str[i];
	}
	newStr[j] = '\x00';
	return newStr;
}

void freeReversedString( unsigned char *str ) {
	free( str );
}



