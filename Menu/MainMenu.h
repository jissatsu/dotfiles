#ifndef __MMAIN_MENU_HH
#define __MMAIN_MENU_HH 1

#include "core/Menu.h"
#include "../config.h"

USING_NS_CC;

class MainMenu : public d_lan::Menu
{
    public:
        static MainMenu * create( Node *parent );
        virtual bool init( void ) override;
        virtual void show( const std::function<void()>& callback, short z_index ) override;
        // virtual void hide( const std::function<void()>& callback, bool remove );
        virtual void add_option( std::shared_ptr<d_lan::SelectionNode> node, const std::string& text, const std::function<void()>& func, bool enabled, short index ) override;
        void update( std::shared_ptr<d_lan::SelectionNode> selection_node );
        void Die( void );
};

#endif
