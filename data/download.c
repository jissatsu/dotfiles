#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "download.h"

int download_ip_file( const char *url, char *fname ) {
    char *err = "[Error] - URL format must be [http://host:port/path] or [http://host:port/]!\n";
    struct url u, *urrl = &u;

    /* host_start is the start index of the host part of the url */
    urrl->entire = (char *) url;
    if ( (urrl->host_start = is_http( urrl->entire, strlen( urrl->entire ) )) > 0 ) {
        parse_host( urrl->entire, urrl->host_start, &u );

        if ( urrl->host != NULL && urrl->port != -1 ) {
            get_path( urrl->entire, urrl->host_start, urrl->host_len, &u );
            get_query_string( urrl->entire, &u );

            if ( urrl->path != NULL ) {
                initiate_download( urrl->entire, urrl->host, urrl->path, urrl->query, urrl->port, fname );
            }
        } else {
            fprintf( stdout, "%s", err );
        }
    } else {
        fprintf( stdout, "%s", err );
    }
    return 0;
}

int initiate_download( const char *url, const char *host, const char *path, const char *query, int port, char *fname ) {
    int sock = 0;
    int opt = 1;
    char host_name[0xFF];
    struct sockaddr_in remote_addr;
    struct in_addr **addr_list;
    struct hostent *hen;
    
    /* extracting the port from the host string */
    if ( strdelimcpy( host_name, (char *) host, ":" ) == -1 ) {
        return -1;
    }

    hen = gethostbyname( host_name );
    if ( hen != NULL ) {
        addr_list = (struct in_addr **) hen->h_addr_list;
        if ( (sock = socket( AF_INET, SOCK_STREAM, 0x00 )) == INVALID_SOCKET ) { 
            fprintf( stderr, "[Error][%s] - Couldn't create socket!\n", __FILE__ );
            return -1;
        }
        setsockopt( sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof( opt ) );

        memset( &remote_addr, '0', sizeof( remote_addr ) );
        remote_addr.sin_family = AF_INET;
        remote_addr.sin_port   = htons( port );

        if ( inet_pton( AF_INET, inet_ntoa( *addr_list[0] ), &remote_addr.sin_addr ) <= 0 ) {
            return -1;
        }

        if ( connect( sock, (struct sockaddr *) &remote_addr, sizeof( remote_addr ) ) != -1 ) {
            request_file_http( sock, path, host_name, query, fname );
        } else {
            fprintf( stderr, "[Error][%s] - Couldn't connect to http server!\n",  __FILE__ );
        }
        return 0;
    }
    return -1;
}

int request_file_http( int sock, const char *path, const char *host, const char *query, const char *fname ) {
    int rpv, sc, nheaders, nagents, resp_len;
    char status[5];
    char data_to_send[BUFSIZ];
    char file_buff[1024], *trimmed;
    struct http_header headers[0xFF];

    rpv = snprintf( data_to_send, sizeof(data_to_send), "GET %s HTTP/1.0\r\nHost: %s\r\n", path, host );
    strcat( data_to_send, "User-Agent: someagent\r\n" );
    strcat( data_to_send, "Connection: close\r\n" );
    strcat( data_to_send, "Accept: */*\r\n\r\n" );

    if ( rpv > 0 ) {
       if ( send( sock, data_to_send, strlen( data_to_send ), 0x00 ) != -1 ) {
           sc = parse_response( sock, headers, &nheaders, status, file_buff, &resp_len );
           if ( sc == 0 ) {
                trimmed = trim( file_buff, strlen( file_buff ) );
                save_http_file( fname, trimmed, resp_len );
                free( trimmed );
                free_headers( headers, nheaders );
           }
       } else {
           fprintf( stderr, "[Error][%s] - Error sending data to socket!\n", __FILE__ );
           return -1;
       }
    }
    return 0;
}

int parse_response( int sock, struct http_header *headers, int *nheaders, char *status, char *response_text, int *response_len ) {
    int rcode, i, j;
    int bytes, num_headers, ntokens;
    char buff[BUFSIZ], *bufp = buff;
    char errbuf[0xFF];
    char *header;

    /* hddr_buff contains the headers part of the reponse */
    char *hddr_buff = (char *) malloc( BUFSIZ * sizeof( char * ) );
    char *he_buf = hddr_buff;

    if ( hddr_buff ) {
        if( (bytes = recv( sock, buff, BUFSIZ, 0x00 ) ) > 0 ) {
            /* separating the headers from the line based text data by `\r\n\r\n` */
            do {
                if ( *bufp == '\r' && *(bufp + 1) == '\n' && *(bufp + 2) == '\r' && *(bufp + 3) == '\n' ) {
                    *he_buf = '\x00';
                    break;
                }
                *he_buf++ = *bufp++;
            } while ( *bufp != '\x00' ) ;
            /**/
            
            char **headerz = (char **) malloc( 0xFF * sizeof( char * ) );
            char **htokens = (char **) malloc( 2 * sizeof( char * ) );

            split_string( hddr_buff, strlen( hddr_buff ), "\r", headerz, &num_headers, errbuf );

            for ( i = 0, j = 0 ; i < num_headers ; i++ ) {
                header = trim( headerz[i], strlen( headerz[i] ) );
                headers[i].header_name  = (char *) malloc( 0xFF * sizeof( char ) );
                headers[i].header_value = (char *) malloc( 2048 * sizeof( char ) );

                split_string( header, strlen( header ), ":", htokens, &ntokens, errbuf );
                memcpy( headers[i].header_name, *htokens, strlen( *htokens ) + 1 );

                if ( strstr( *htokens, "HTTP/" ) != NULL ) {
                    memset( headers[i].header_value, '\x00', 1 );
                } else {
                    memcpy( headers[i].header_value, *(htokens + 1), strlen( *(htokens + 1) ) + 1 );
                }
            }
            /* continue from where we left off */
            int clen = 0;
            bufp += 4;
            do {
                *response_text++ = ( *bufp == '\x00' ) ? '\x00' : *bufp++; clen++;
            } while ( *bufp != '\x00' ) ;
            /**/

            *nheaders = num_headers, *response_len = clen;
            free( hddr_buff );
            free_header_tokens( htokens, ntokens );
            return 0;
        } else {
            fprintf( stderr, "[Error][%s] - Error reading from socket!\n", __FILE__ );
        }
    }
    return -1;
}

int parse_host( const char *url, int host_start_index, struct url *urrl ) {
    int i, j, port_i, host_end = 0;
    char port[100];
    static char host_part[0xFF];

    if ( url && host_start_index ) {
        for ( i = host_start_index, j = 0 ; url[i] != '\x00' ; i++ ) {
            if ( url[i] == '/' ) {
                host_part[j] = '\x00', urrl->host = host_part, host_end = 1;
                urrl->host_len = strlen( host_part );
                break;
            } else {
                host_part[j++] = url[i];
            }
        }
        port_i = strpos( urrl->host, ":" );
        if ( host_end && urrl->host && (port_i > 0) ) {
            for ( i = port_i + 1, j = 0 ; urrl->host[i] != '\x00' ; i++ ) {
                port[j++] = urrl->host[i];
            }
            if ( strlen( port ) > 0 ) {
                port[j] = '\x00', urrl->port = atoi( port );
                return 0;
            }
        }
    }
    urrl->host = NULL, urrl->port = -1;
    return -1;
}

int get_path( const char *url, int host_name_start, int host_name_end, struct url *urrl ) {
    int i, j;
    static char path[BUFSIZ];

    if ( url && host_name_end ) {
        for ( i = (host_name_start + host_name_end), j = 0 ; url[i] != '\x00' ; i++ ) {
            path[j++] = url[i];
        }
        path[j] = '\x00', urrl->path = path;
        return 0;
    }
    urrl->path = NULL;
    return -1;
}

int get_query_string( const char *url, struct url *urrl ) {
    int qstart, i, j;
    static char query[BUFSIZ];

    if ( url ) {
        if ( (qstart = strpos( url, "?" )) > 0 ) {
            for ( i = qstart, j = 0 ; url[i] != '\x00' ; i++ ) {
                query[j++] = url[i];
            }
            query[j] = '\x00', urrl->query = query;
            return 0;
        }
    }
    urrl->query = NULL;
    return -1;
}

int is_http( const char *url, size_t len )  {
    int i, j, k, found = 0;
    char proto[] = "http://";

    for ( i = 0 ; i < len ; i++ ) {
        for ( j = i, k = 0 ; proto[k] != '\x00' && proto[k] == url[j] ; j++, k++ ) {
            ;
        }
        if ( proto[k] == '\x00' ) {
            return k;
        }
    }
    return -1;
}

int save_http_file( const char *fname, char *buff, int nbytes ) {
    int i = 0;
    FILE *fp;
    if ( (fp = fopen( fname, "w" )) != NULL ) {
        while ( *buff != '\x00' & i < nbytes ) {
            fputc( *buff, fp ), buff++, i++;
        }
        fclose( fp ); return 0;
    }
    return -1;
}

void free_headers( struct http_header *headers, int num_headers ) {
    for ( int i = 0 ; i < num_headers ; i++ ) {
        free( headers[i].header_name );
        free( headers[i].header_value );
    }
}

void free_header_tokens( char **htokens, int num_tokens ) {
    for ( int i = 0 ; i < num_tokens ; i++ ) {
        free( htokens[i] );
    }
}