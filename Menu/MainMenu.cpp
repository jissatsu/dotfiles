#include "MainMenu.h"

MainMenu * MainMenu::create( Node *parent )
{
    MainMenu *menu = new MainMenu();
    if ( menu && menu->init( parent ) ) {
        menu->autorelease();
        return menu;
    }
    CC_SAFE_DELETE( menu );
    return nullptr;
}

bool MainMenu::init( Node *parent ) {
    if ( !parent ) {
        return false;
    }
    _parent = parent;
    _listView = std::shared_ptr<OptionListView>(new OptionListView(this, UI::MainMenu::optionLV, {240.0f, 65.0f}));
    // initialize menu cursor
    _cursor = std::shared_ptr<Cursor>(
        new Cursor(_listView->GetLayer(), {0.5f, -12.0f}, 0.7f, 0.9f, UI::MainMenu::cursor));
    _cursor->pulse( 0.2f );
    return true;
}

void MainMenu::add_option( const std::string& text, const std::function<void()>& func, bool enabled, short index ) {
    _selections.push_back(
        std::move( std::shared_ptr<d_lan::Option>(
            new d_lan::Option(
                _listView->GetLayer(), text, func, enabled, index
            )
        ))
    ); update();
}

void MainMenu::update( void ) {
    auto cmd_pos = _listView->GetLayer()->getPosition();
    short count = _selections.size();
    for ( auto& selection : _selections ) {
        selection->getLayer()->setPosition( Vec2( (cmd_pos.x - 240) / 2, (12.3f * count) - 24 ) );
        count--;
    }
}
