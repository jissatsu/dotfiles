#ifndef __PIDMAP_PARSER
#define __PIDMAP_PARSER 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdarg.h>
#include <elf.h>

#ifdef __cplusplus
extern "C" {
#endif

#define pidmap__set_err( str, ret ){\
    sprintf( pidmap_err, "%s", str ); return ret;\
}

char pidmap_err[0xff];

typedef enum {
    PIDMAP_EXEC,
    PIDMAP_SHAREDLIB,
    PIDMAP_STACK,
    PIDMAP_HEAP,
    PIDMAP_VDSO
} vma_filetype_t;

typedef enum { R, RW, RWX } vma_perm_t;

struct vma_file {
    vma_filetype_t _type;
    char name[0xff];
};

struct vma_list {
    unsigned long start;
    unsigned long end;
    vma_perm_t perms;
    unsigned long offset;
    char inode[10];
    struct vma_file _file;
    struct vma_list *next;
};

struct vma_list * pidmap__get_maps( pid_t pid );
struct vma_list * __new_vma_entry( struct vma_list *current );
void __add_vma( struct vma_list *vma, char *buff );
void __release( struct vma_list *vma_list );
int real_pid( pid_t pid );
char * pidmap__get_err( void );

#ifdef __cplusplus
}
#endif

#endif