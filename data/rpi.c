#include "log.h"
#include "rpi.h"

int main( int argc, char **argv )
{
    libnet_t *ltag;
    uint8_t ip[16];
    int opt;
    char *device;
    char *server;
    char *port;
    char err_buf[LIBNET_ERRBUF_SIZE];

    ltag   = NULL;
    device = NULL;
    server = NULL;
    port   = NULL;

    while( (opt = getopt( argc, argv, "d:s:p:" )) != -1 ){
        switch( opt )
        {
            case 'd': device = optarg; break;
            case 's': server = optarg; break;
            case 'p': port   = optarg; break;
            default:
                __rpi_usage( argv[0] );
                exit( RPI_BAD );
        }
    }

    if ( server == NULL || port == NULL )
        __rpi_die( "Server ip or port not specified!\n" );

    ltag = __rpi_init( rpi_fp, device, err_buf );
    if ( ltag == NULL )
        __rpi_die( err_buf );
    
    __rpi_getaddr( ltag, ip, RPI_IPV4, err_buf );
    __rpi_arp_initiate( ltag, ip );
    return RPI_OK;
}


/* initialize libnet context and rpi log file */
libnet_t * __rpi_init( FILE *rpi_log_fp, char *device, char *err )
{
    libnet_t *ltag;
    FILE *fp;

    ltag = libnet_init( LIBNET_LINK, device, err );
    if ( ltag == NULL ){
        return NULL;
    }

    fp = fopen( RPI_LOG_FILE, "a+" );
    if ( fp == NULL ) {
        sprintf( err, "%s\n", strerror( errno ) );
        return NULL;
    }

    rpi_log_fp = fp;
    __rpi_log( 
        rpi_log_fp, RPI_LOG_INFO, "Libnet successfully initialized!\n"
    );

    if ( device == NULL ){
        __rpi_log( 
            rpi_log_fp, RPI_LOG_INFO, "Device autodetect!\n"
        );
    }
    return ltag;
}


uint8_t __rpi_build_packet( uint8_t *src_ip, uint8_t *dst_ip, 
                            uint8_t *src_mac, char *device, char *err )
{
    libnet_t *ztag;
    libnet_ptag_t ether, arp;

    ztag = libnet_init( LIBNET_LINK, device, err );
    if ( ztag == NULL ) {
        return RPI_BAD;
    }

    arp = libnet_build_arp(
        ARPHRD_ETHER,
        ETHERTYPE_IP,
        6,
        4,
        ARPHDR_OP,
        src_mac,
        src_ip,
        arp_hw_dest,
        dst_ip,
        NULL,
        0,
        ztag,
        0
    );

    if ( arp == -1 ) {
        sprintf( err, "%s", "__rpi_build_packet(): ARP packet error!\n" );
        return RPI_BAD;
    }

    ether = libnet_build_ethernet(
        eth_hw_dest,
        src_mac,
        ETHERTYPE_ARP,
        NULL,
        0,
        ztag,
        0
    );

    if ( ether == -1 ) {
        sprintf( err, "%s", "__rpi_build_packet(): Ethernet frame error!\n" );
        return RPI_BAD;
    }
    return RPI_OK;
}


uint8_t __rpi_arp_initiate( libnet_t *ltag, uint8_t *ip )
{
    uint8_t nhosts;
    uint8_t arp_stat;
    uint8_t *src_mac, src_ip[4], *dst_ip, netmask[16];
    uint32_t start_ip;
    char err_buf[LIBNET_ERRBUF_SIZE];

    src_mac = __rpi_gethwaddr( ltag, err_buf );
    if ( src_mac == NULL ){
        __rpi_die( err_buf );
    }

    __rpi_getaddr( ltag, netmask, RPI_MASK, err_buf );
    if ( netmask == NULL ) {
        __rpi_die( err_buf );
    }

    __rpi_normalize_ip( ip, src_ip );
    nhosts   = __rpi_calc_nhosts( netmask );
    start_ip = __rpi_calc_net_start_off( ip, netmask );
    return RPI_OK;
}


/* get ip address or netmask of device */
uint8_t __rpi_getaddr( libnet_t *ltag, uint8_t *entity, 
                       rpi_adr_t type, char *err )
{
    int sock;
    const char *dev;
    u_short iostat;
    u_long req;
    struct ifreq iface;
    struct sockaddr_in *ipdata;

    sock = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sock == -1 ) {
        sprintf( err, "%s", strerror( errno ) );
        return RPI_BAD;
    }
    
    dev = libnet_getdevice( ltag );

    switch ( type )
    {
        case RPI_IPV4: req = SIOCGIFADDR;    break;
        case RPI_MASK: req = SIOCGIFNETMASK; break;
        default:
            sprintf( err, "Invalid rpi_adr_t!\n" );
            return RPI_BAD;
    }
    
    memcpy( iface.ifr_name, dev, strlen( dev ) + 1 );
    iostat = ioctl( sock, req, &iface );
    if ( iostat == -1 )
    {
        sprintf( err, "%s", strerror( errno ) );
        return RPI_BAD;
    }
    
    ipdata = (struct sockaddr_in *) &iface.ifr_addr;
    memcpy( 
        entity, inet_ntoa( ipdata->sin_addr ), 16 
    );
    close( sock );
    return RPI_OK;
}


/* get the MAC address of the device */
uint8_t * __rpi_gethwaddr( libnet_t *ltag, char *err  )
{
    static uint8_t mac[6];
    struct libnet_ether_addr *addr;

    addr = libnet_get_hwaddr( ltag );
    if ( addr == NULL )
    {
        sprintf( err, "%s\n", libnet_geterror( ltag ) );
        return NULL;
    }
    memcpy( mac, addr->ether_addr_octet, 6 );
    return mac;
}


void __rpi_usage( char *prog )
{
    printf( "Usage: %s -d [devname] -s [server_ip]\n", prog );
}


void __rpi_die( char *err )
{
    __rpi_log( stderr, RPI_LOG_ERR, err );
    exit( RPI_BAD );
}