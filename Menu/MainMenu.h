#ifndef __MMAIN_MENU_HH
#define __MMAIN_MENU_HH 1

#include "core/Menu.h"
#include "../config.h"

USING_NS_CC;

class MainMenu : public d_lan::Menu
{
    public:
        static MainMenu * create( Node *parent );
        virtual bool init( Node *parent ) override;
        virtual void add_option( std::shared_ptr<d_lan::SelectionNode> node, const std::string& text, const std::function<void()>& func, bool enabled, short index ) override;
        void update( std::shared_ptr<d_lan::SelectionNode> selection_node );
        void Die( void );
};

#endif
