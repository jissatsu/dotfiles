#include "includes/arp.h"

// ethernet frame broadcast address
static const uint8_t ether_broadcast_hw[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

// arp unknown hardware address
static const uint8_t arp_broadcast_hw[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static session_t _session;

bool arp_build_packet( const struct _arp *arpdata, libnet_t *ltag ) {
    libnet_ptag_t ether, arp;
    ether = arp = LIBNET_PTAG_INITIALIZER;

    arp = libnet_autobuild_arp(
        ARPOP_REQUEST,
        arpdata->source_hw,
        arpdata->source_ip,
        arp_broadcast_hw,
        arpdata->target_ip,
        ltag
    );
    if ( arp < 0 ) {
        dlan_error::set( __FILE__, libnet_geterror( ltag ) );
        return false;
    }

    ether = libnet_autobuild_ethernet( ether_broadcast_hw, ETHERTYPE_ARP, ltag );
    if ( ether < 0 ) {
        dlan_error::set( __FILE__, libnet_geterror( ltag ) );
        return false;
    }
    return true;
}

bool arp_start_session( const net_t *net, string& err, libnet_t *ltag ) {
    struct _arp arp;

    _session.reset();
    return true;
}

/* check if the current session has ended */
bool session_ended( void ) {
    return _session.state == "end" ? true : false ;
}