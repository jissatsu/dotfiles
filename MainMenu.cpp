#include "MainMenu.h"

MainMenu * MainMenu::create( void )
{
    MainMenu *menu = new MainMenu();
    if ( menu ) {
        menu->autorelease();
        return menu;
    }
    CC_SAFE_DELETE( menu );
    return nullptr;
}

bool MainMenu::init( Node *parent ) {
    _parent = parent;
    if ( !init_bg() ) { return false; }
    return true;
}

bool MainMenu::init_bg( void )
{
    _cmd_container = Sprite::create( UI::MainMenu::options );
    if ( !_cmd_container ) {
        return false;
    }
    auto director = Director::getInstance();
    _cmd_container->setAnchorPoint(
        Vec2(
            0.5f, 0.5f
        )
    );
    _cmd_container->setPosition(
        Vec2(
             director->getVisibleOrigin().x +  director->getVisibleSize().width  / 2,
            (director->getVisibleOrigin().y - 70) +  director->getVisibleSize().height / 2
        )
    );
    addChild( _cmd_container );
    return true;
}

/* initialize menu cursor */
bool MainMenu::cursor( void )
{
    _cursor = Cursor::create(
        _cmd_container, { 39.5f, 37.0f }, UI::MainMenu::cursor );
    if ( !_cursor ) {
        return false;
    }
    _cursor->setScaleX( 0.7f );
    _cursor->setScaleY( 0.9f );
    _cursor->pulse( 0.2f );
    return true;
}

void MainMenu::add_option( const std::string& text, const std::function<void()>& func, bool enabled, short index ) {
    _selections.push_back(
        std::move( std::shared_ptr<d_lan::Option>(
            new d_lan::Option(
                _cmd_container, text, func, enabled, index
            )
        ))
    ); update();
}

void MainMenu::update( void ) {
    auto cmd_size = _cmd_container->getContentSize();
    short count = _selections.size();
    for ( auto& selection : _selections ) {
        selection->getLayer()->setPosition( Vec2( cmd_size.width / 2, 12.3f * count ) );
        count--;
    }
}