#include "includes/dlan.h"

std::shared_ptr<dlan> dlan::_instance = nullptr;

dlan::dlan( void ) : _wlist(false){}

dlan::~dlan(){}

std::shared_ptr<dlan> dlan::instance( void ) {
    if ( !_instance.get() ) {
        _instance = std::shared_ptr<dlan>(new dlan());
    }
    return _instance;
}

bool dlan::init( const char *dev, const char *netrange, net_t *net ) {
    if ( !(libnet_ctx = libnet_init( LIBNET_LINK, dev, dlan_error::get() )) ) {
        return false;
    }
    if ( (net->ip = libnet_get_ipaddr4( libnet_ctx )) < 0 ) {
        dlan_error::set( __FILE__, libnet_geterror( libnet_ctx) );
        return false;
    }
    struct libnet_ether_addr *hw;
    if ( !(hw = libnet_get_hwaddr( libnet_ctx )) ) {
        dlan_error::set( __FILE__, libnet_geterror( libnet_ctx) );
        return false;
    }
    memcpy( net->hw, hw->ether_addr_octet, 0x6 );
    if ( !init_net_range( netrange, net ) ) {
        return false;
    }
    return true;
}


bool dlan::init_net_range( const char *netrange, net_t *net ) {
    std::vector<std::string> netinfo = split_str( netrange, '/' );
    const char *net_ip = netinfo[0].c_str();
    short subnet = atoi( netinfo[1].c_str() );

    if ( !is_ipv4( net_ip ) ) {
        dlan_error::set( __FILE__, "Invalid ip format in netrange!\n" );
        return false;
    }
    net->mask = retreive_mask( subnet );
    net->nhosts = net->mask ^ 0xffffffff;
    return true;
}


bool dlan::start( const net_t *net ) {
    if ( !start_sniffer( net ) ) {
        dlan_error::set( __FILE__, strerror( errno ) );
        return false;
    }
    dlan_log::log( dlan_log::DEBUG, "%s: %s", __FILE__, "Waiting for the sniffer!\n" );
    while ( !receiver::is_ready() )
        ;
    std::string err;
    for ( ;; ) {
        if ( !session_ended() ) {
            if ( !arp_start_session( net, err, libnet_ctx ) ) {
                dlan_error::set( "arp.cpp", err.c_str() );
                return false;
            }
        }
        sleep( SESSION_DELAY );
    }
    return true;
}

/*
 * start the arp sniffer
 */
bool dlan::start_sniffer( const net_t *net ) const {
    pthread_t thread;
    int status = pthread_create( &thread, NULL,
        receiver::start,
        (void *) net
    );
    if ( status != 0 ) {
        errno = status; return false;
    }
    dlan_log::log( dlan_log::DEBUG, "%s: %s", __FILE__, "Arp sniffer started successfully!\n" );
    return true;
}

/*
 * initialize the whitelist
 */
bool dlan::set_whitelist( const char *filepath ) {
    _whitelist = FS::import( filepath );
    if ( _whitelist.empty() ) {
        dlan_error::set( __FILE__, "Whitelist is empty!\n" );
        return false;
    }
    for ( auto& entry : _whitelist ) {
        if ( !is_valid_hw( entry.c_str() ) ) {
            dlan_error::set( __FILE__, "Invalid hw address format found in whitelist!\n" );
            return false;
        }
    }
    _wlist = true; return true;
}

libnet_t * dlan::get_libnet_context( void ) {
    return _instance->libnet_ctx;
}

/*  */
void dlan::end( int signum ) {
    libnet_t *ctx = dlan::get_libnet_context();
    libnet_destroy( ctx );
    std::exit(0x00);
}
