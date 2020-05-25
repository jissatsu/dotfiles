#include "includes/util.h"

/*
 * Split a string into a vector of tokens
 * @param const string& str
 * @param const char delim
 * @return vector<string>
 */
vector<string> split_str( const string& str, const char delim ) {
    if ( str.empty()) {
        return {};
    }
    stringstream ss(str);
    string s;
    vector<string> tokens;

    while ( getline( ss, s, delim ) ) {
        if ( !s.empty() ) {
            tokens.push_back( s );
        }
    }
    return tokens;
}

/*
 * Remove any trailing and leading whitespace characters from a char array
 * @param const char *str
 * @return string
 */
string trim( const char *str ) {
    if ( !str ) {
        return "";
    }
    string newstr;
    while ( isspace( *str ) ) {
        str++;
    }
    newstr = string(str);
    return newstr;
}

/*
 * Check if the string is valid hardware address
 * @param const char *str
 * @return bool
 */
bool is_valid_hw( const char *str ) {
    unsigned int hw[0x6];
    int scan = sscanf(str, "%x:%x:%x:%x:%x:%x", &hw[0], &hw[1], &hw[2], &hw[3], &hw[4], &hw[5] );
    if ( scan == EOF ) {
        return false;
    }
    for ( int i = 0 ; i < 0x06 ; i++ ) {
        if ( hw[i] < 0x00 || hw[i] > 0xff ) {
            return false;
        }
    }
    return true;
}

/*
 * Convert an ip address from a dotted string into uint32_t
 * @param const char *ip
 * @return uint32_t
 */
uint32_t iptol( const char *ip ) {
    uint32_t _long = 0;
    char frag[4];
    uint8_t rtv[4];

    if ( ip ) {
        for ( int i = 0, j = 0, k = 0 ; i < 4 ; i++ ) {
            frag[j]     = (isdigit( *ip )) ? *ip++ : '\0';
            frag[j + 1] = (isdigit( *ip )) ? *ip++ : '\0';
            frag[j + 2] = (isdigit( *ip )) ? *ip++ : '\0';
            frag[j + 3] = '\0';
            ip++;
            rtv[k++] = atoi( frag );
        }
    }
    _long = (rtv[0] << 24) | (rtv[1] << 16) |
            (rtv[2] <<  8) | (rtv[3] <<  0);
	return _long;
}

/*
 * Convert an ip address from a uint32_t into a dotted string
 * @param const char *ip
 * @return uint32_t
 */
void ltoip( const uint32_t lng, char *str ) {
    char ip[16];
    uint8_t arr[4];
    short off = 24;

    for ( int i = 0, k = 0 ; i < 0x4 ; i++ ) {
        arr[k++] = (lng >> off) & 0xff;
        off -= 8;
    }
    sprintf( ip, "%d.%d.%d.%d", arr[0], arr[1], arr[2], arr[3] );
    memcpy( str, ip, sizeof(ip) );
}

/*
 * Retrieve netmask from subnet
 * /16 -> 255.255.0.0
 * /24 -> 255.255.255.0 etc...
 */
uint32_t sub_mask( short subnet ) {
    if ( subnet < 16 || subnet > 30 ) {
        return -1;
    }
    short max    = 32; /* netmask max size is 32 bits */
    short offset = max - subnet;
    uint32_t lng = (0xffffffff >> offset) << offset;
    return lng;
}