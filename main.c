#include "pidmap.h"

static void __die( const char *str ) {
    printf( "%s\n", str );
    exit( 0x02 );
}

int main( int argc, char **argv )
{
    pid_t pid = getpid();
    struct vma_list *vma_list = pidmap__get_maps( pid );
    if ( !vma_list ) {
        __die( pidmap__get_err() );
    }
    while ( vma_list != NULL ) {
        if ( vma_list->_file._type == PIDMAP_SHAREDLIB && vma_list->perms == RWX ) {
            break;
        }
        vma_list = vma_list->next;
    }
    return 0;
}