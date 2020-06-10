#include "pidmap.h"

void __parse_line( const char *buff, struct vma_list *vma )
{
    char *token = strtok( buff, " " );
    while ( token != NULL ) {
        token = strtok( NULL, " " );
    }
}

uintptr_t __parse_hex( const char *str ) {
    return 0;
}

static void __define_perms( struct vma_list *vma_list, char *perms ) {
    if ( perms[0] == 'r' )
        vma_list->perms |= R;

    if ( perms[1] == 'w' )
        vma_list->perms |= W;

    if ( perms[2] == 'x' )
        vma_list->perms |= X;

    if ( perms[3] == 'p' )
        vma_list->perms |= P;
}

struct vma_list * pidmap__get_maps( pid_t pid )
{
    char path[20];
    sprintf( path, "/proc/%d/maps", pid );
    
    FILE *fp = fopen( path, "r" );
    if ( !fp ) {
        pidmap__set_err(
            strerror( errno ),
            NULL
        );
    }
    struct vma_list *vma_list = __new_vma_entry( NULL );
    if ( !vma_list )
        return NULL;

    char buff[0xFF];
    while ( fgets( buff, sizeof( buff ), fp ) ) {
        if ( __add_vma( vma_list, buff ) < 0 ) {
            return NULL;
        }
    }
    return vma_list;
}

int __add_vma( struct vma_list *vma, char *buff )
{
    struct vma_list *vma_entry;
    vma_entry = vma;

    while ( vma_entry->next != NULL )
        vma_entry = vma_entry->next;
    
    __parse_line( buff, vma_entry );

    if ( !(vma_entry->next = __new_vma_entry( vma_entry )) ) {
        return -1;
    }
    return 0;
}

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

void pidmap__release( struct vma_list *vma_list )
{
    struct vma_list *cursor, *temp;
    cursor = vma_list->next;
    vma_list->next = NULL;

    while ( cursor ) {
        temp = cursor->next, free( temp ), cursor = temp;
    }
}

char * pidmap__get_err( void ) {
    return pidmap_err;
}