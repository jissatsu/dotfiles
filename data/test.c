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
	unsigned char url[0xFF] = "http://www.cyber-chann.bg/user?uid=1wha-tis that man_";
	unsigned char *encoded;

	encoded = urlencode( url );
	fprintf( stdout, "%s\n", encoded );
	return 0;
}

unsigned char * urlencode( unsigned char const *str ) {
	int i, j, k;
	size_t len;
	unsigned char *c;
	static unsigned char *encoded;
	char chars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	
	i = j = 0;
	len = strlen( str );
	encoded = (unsigned char *) malloc( (len * 3) + 1 );
	
	while ( str[i] != '\x00' ) {
		if ( str[i] == ' ' ) {
			encoded[j++] = '+';
		}
		else if ( 
			(str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') ||
			(str[i] >= '0' && str[i] <= '9') ||
			 str[i] == '.' || str[i] == '~'  ||  str[i] == '-' || str[i] == '_' ) {
			encoded[j++] = str[i];
		} 
		else{
			encoded[j++] = '%';
			encoded[j++] = toupper( chars[ str[i] >> 4 ] );
			encoded[j++] = toupper( chars[ str[i] & 15 ] );
		}
		i++;
	}
	encoded[j++] = '\x00';
	return encoded;
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



