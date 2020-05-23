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

bool dlan::init( const char *dev, const char *iprange, net_t *net ) {
    /* if ( !parse_net( iprange, net ) ) {
        dlan_error::set( __FILE__, "Invalid IP range!\n" );
        return false;
    } */
    if ( !(libnet_ctx = libnet_init( LIBNET_LINK, dev, dlan_error::get() )) ) {
        return false;
    }
    if ( (net->ip = libnet_get_ipaddr4( libnet_ctx )) < 0 ) {
        dlan_error::set( __FILE__, libnet_geterror( libnet_ctx) );
        return false;
    }
    net->iface = dev ? dev : nullptr ;
    // net->nhosts = calc_hosts();
    return true;
}

void dlan::start( const net_t *net ) {
    /* if ( !start_sniffer( net ) ) {
        dlan_error::set( __FILE__, "Couldn't spawn arp sniffer!\n" );
        return false;
    }
    while ( !receiver::is_ready() )
        ;
    for ( ;; ) {
        if ( !inject_arp( net, _errbuf ) ) {
            return false;
        }
    } return true; */
}

bool dlan::start_sniffer( const net_t *_net ) const {
    /* pthread_t thread;
    int err = pthread_create(
        &thread,
        NULL,
        sniffer,
        (void *) _net
    );
    if ( err ) {
        return false;
    } */
    dlan_log::log( dlan_log::DEBUG, "%s: %s", __FILE__, "Arp sniffer started successfully!\n" );
    return true;
}

/* initialize the whitelist */
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
    libnet_clear_packet( ctx );
    libnet_destroy( ctx );
    std::exit(0x00);
}