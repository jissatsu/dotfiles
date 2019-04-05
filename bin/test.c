#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>

#define FILE_0 "test_file1.c"  /* type in whatever you want */
#define FILE_1 "test_file2.c"  /* type in whatever you want */

int filesize( int fd );
int filecomp( int fd1, int fd2, char *line );

int main( int argc, char *argv[] ) {
    int fd1, fd2, rcode;
    char line[BUFSIZ];

    if ( (fd1 = open( FILE_0, O_RDONLY, 0x00 )) == -1 ) {
        fprintf( stdout, "Error opening file: %s\n", FILE_0 );
        exit( EXIT_FAILURE );
    }
    if ( (fd2 = open( FILE_1, O_RDONLY, 0x00 )) == -1 ) {
        fprintf( stdout, "Error opening file: %s\n", FILE_1 );
        exit( EXIT_FAILURE );
    }

    rcode = filecomp( fd1, fd2, line );
    if ( strlen( line ) > 0 ) {
        fprintf( stdout, "%s\n", line );
    } else {
        fprintf( stdout, "Files are either both 0MB or equal.\n" );
    }
    exit( 0 );
}

int filecomp( int fd1, int fd2, char *line ) {
    int line_num;
    char line1[BUFSIZ];
    char line2[BUFSIZ];
    FILE *fp1, *fp2;

    if ( filesize( fd1 ) <= 0 && filesize( fd2 ) <= 0 ) {
        strcpy( line, "" );
        return 0;
    }
    fp1 = fdopen( fd1, "r" );
    fp2 = fdopen( fd2, "r" );
    
    line_num = 0;
    while ( fgets( line1, BUFSIZ, fp1 ) != NULL && fgets( line2, BUFSIZ, fp2 ) != NULL ) {
        if ( strcmp( line1, line2 ) < 0 ) {
            strcpy( line, "[FILE] - " );
            strcat( line, FILE_0 );
            strcat( line, "\n" );
            strcat( line, line1 );
            return 1;
        } 
        else if ( strcmp( line1, line2 ) > 0 ) {
            strcpy( line, "[FILE] - " );
            strcat( line, FILE_1 );
            strcat( line, "\n" );
            strcat( line, line2 );
            return 1;
        }
        line_num++;
    }
    strcpy( line, "" );
    return 0;
}

int filesize( int fd ) {
    FILE *fp;
    int c, i = 0;

    if ( (fp = fdopen( fd, "r" )) != NULL ) {
        while ( ( c = getc( fp ) ) != EOF && !feof( fp ) )
            i++;
        fseek( fp, 0, SEEK_SET );
        return i;
    }
    return -1;
}