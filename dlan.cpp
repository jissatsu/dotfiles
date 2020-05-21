#include "includes/dlan.h"

std::shared_ptr<DLAN> DLAN::_instance = nullptr;

DLAN::DLAN( void ){}

DLAN::~DLAN(){}

std::shared_ptr<DLAN> DLAN::instance( void ) {
    if ( !_instance.get() ) {
        _instance = std::shared_ptr<DLAN>(new DLAN());
    }
    return _instance;
}

bool DLAN::init( const char *dev, const char *iprange, net_t *net ) {
    if ( !dev ) {
        ERROR(ERRDEVICE);
    }
    if ( !(libnet_ctx = libnet_init( LIBNET_LINK, dev, _errbuf )) ) {
        return false;
    }
    return true;
}

/* initialize the whitelist */
bool DLAN::set_whitelist( const char *filepath ) {
    _whitelist = FS::import( filepath );
    if ( _whitelist.empty() ) {
        ERROR(ERRWLIST);
    }
    for ( const auto& entry : _whitelist ) {
        if ( !is_valid_hw(entry.c_str()) ) {
            ERROR(ERRHWFORMAT);
        }
    }
    return true;
}

void DLAN::start( const net_t *net ) {
    ;
}

bool DLAN::start_receiver( const net_t *net ) const {
    return true;
}

/* get the error buffer */
const char * DLAN::get_last_err( void ) const {
    return _errbuf;
}

libnet_t * DLAN::get_libnet_context( void ) {
    return _instance->libnet_ctx;
}

/*  */
void DLAN::end( int signum ) {
    libnet_t *ctx = DLAN::get_libnet_context();
    libnet_clear_packet( ctx );
    libnet_destroy( ctx );
    std::exit(0x00);
}