#include "pidmap.h"

struct vma_list * pidmap__get_maps( pid_t pid )
{
    char path[20];
    sprintf( path, "/proc/%d/maps", pid );
    FILE *fp = fopen( path, "r" );
    if ( !fp ) {
        pidmap__set_err( strerror( errno ), NULL );
    }
    struct vma_list *vma_list = __new_vma_entry( NULL );
    if ( !vma_list )
        return NULL;

    char buff[0xFF];
    while ( fgets( buff, sizeof( buff ), fp ) ) {
        __add_vma( vma_list, buff );
    }
    return vma_list;
}

/* add a vma_list entry to the vma list */
void __add_vma( struct vma_list *vma, char *buff )
{
    ;
}

/* create a new vma_list entry */
struct vma_list * __new_vma_entry( struct vma_list *current )
{
    struct vma_list *vma_list = (struct vma_list *) malloc( sizeof(struct vma_list) );
    if ( !vma_list )
        pidmap__set_err( strerror( errno ), NULL );

    if ( current ) {
        current->next = vma_list;
    }
    vma_list->next = NULL;
    memset( vma_list->_file.name, '\x00', 0xFF );
    return vma_list;
}

/* release allocated vma list */
void __release( struct vma_list *vma_list )
{
    struct vma_list *cursor, *temp;
    cursor = vma_list->next;
    vma_list->next = NULL;

    while ( cursor ) {
        temp = cursor->next, free( temp );
        cursor = temp;
    }
}

char * pidmap__get_err( void ) {
    return pidmap_err;
}