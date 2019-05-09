#include "alloc.h"

char * alloc_chrmem( size_t size ) {
    void *loc;
    if ( size <= 0 ) {
        fprintf( stderr, "alloc_chrmem() - Invalid size!\n" );
        exit( EXIT_FAILURE );
    }

    if ( (loc = malloc( size * sizeof( char ) )) != NULL ) {
        memset( loc, '0', size );
    } else {
        fprintf( stderr, "alloc_chrmem() - Couldn't allocate memory!\n" );
        exit( EXIT_FAILURE );
    }
    return (char *) loc;
}

char ** alloc_pchrmem( size_t size ) {
    void **loc;
    if ( size <= 0 ) {
        fprintf( stderr, "alloc_pchrmem() - Invalid size!\n" );
        exit( EXIT_FAILURE );
    }

    if ( (loc = malloc( size * sizeof( char * ) )) != NULL ) {
        memset( loc, '0', size );
    } else {
        fprintf( stderr, "alloc_pchrmem() - Couldn't allocate memory!\n" );
        exit( EXIT_FAILURE );
    }
    return (char **) loc;
}

uint8_t * alloc_uchrmem( size_t size ) {
    void *loc;
    if ( size <= 0 ) {
        fprintf( stderr, "alloc_uchrmem() - Invalid size!\n" );
        exit( EXIT_FAILURE );
    }

    if ( (loc = malloc( size * sizeof( uint8_t ) )) != NULL ) {
        memset( loc, '0', size );
    } else {
        fprintf( stderr, "alloc_uchrmem() - Couldn't allocate memory!\n" );
        exit( EXIT_FAILURE );
    }
    return (uint8_t *) loc;
}
