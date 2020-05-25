#include "includes/log.h"

namespace dlan_log {

    static FILE *_fp = nullptr;
    static const char *_dateformat = "%m/%d/%Y - %H:%M:%S";

    /* initialize the log file */
    bool init( const char *filepath ) {
        std::stringstream errstr;
        _fp = fopen( filepath, "a+" );
        if ( !_fp ) {
            errstr << filepath << " - " << strerror(errno);
            dlan_error::set( __FILE__, errstr.str().c_str() );
            return false;
        }
        return true;
    }

    struct tm * get_time( void ) {
        time_t rawtime;
        time( &rawtime );
        return (localtime( &rawtime ));
    }

    void log( log_level_t level, const char *format, ... ) {
        const char *lvl = nullptr;
        char buff1[100] = {0},
             buff2[100] = {0};

        strftime( buff1, 100, _dateformat, get_time() );
        va_list valist;

        switch ( level ) {
            case DEBUG:
                lvl = "[DEBUG]   - ";
                break;
            case WARNING:
                lvl = "[WARNING] - ";
                break;
            case ERROR:
                lvl = "[ERROR]   - ";
                break;
        }

        va_start( valist, format );
        vsprintf( buff2, format, valist );
        va_end( valist );
        fprintf( _fp, "%s %s %s", lvl, buff1, buff2 );
        fflush( _fp );
    }
};