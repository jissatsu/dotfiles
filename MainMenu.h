#ifndef __MMAIN_MENU_HH
#define __MMAIN_MENU_HH 1

#include "MenuCore.h"
#include "../config.h"

USING_NS_CC;

class MainMenu : public MenuCore
{
    public:
        static MainMenu * create( Node *parent );
        virtual bool init( Node *parent ) override;
        virtual void add_option( const std::string& text, const std::function<void()>& func, bool enabled, short index ) override;
        void update( void );
        void Die( void );
};

#endif
