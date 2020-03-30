#ifndef __MMAIN_MENU_HH
#define __MMAIN_MENU_HH 1

#include "AbstractMenu.h"
#include "OptionListView.h"
#include "../config.h"

USING_NS_CC;

class MainMenu : public AbstractMenu
{
    public:
        static MainMenu * create( Node *parent );
        virtual bool init( Node *parent ) override;
        virtual void add_option( const std::string& text, const std::function<void()>& func, bool enabled, short index ) override;
        void update( void );
        void Die( void );

    private:
        std::shared_ptr<OptionListView> _listView;
};

#endif