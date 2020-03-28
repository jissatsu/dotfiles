#include "MainMenu.h"

MainMenu * MainMenu::create( void )
{
    MainMenu *menu = new MainMenu();
    if ( menu ) {
        menu->_director = Director::getInstance();
        menu->autorelease();
        return menu;
    }
    CC_SAFE_DELETE( menu );
    return nullptr;
}

bool MainMenu::init_bg( void )
{
    _cmd_container = Sprite::create( "../Resources/tiles/text_boxes/menu/mainmenu2.png" );
    if ( !_cmd_container ) {
        return false;
    }
    _cmd_container->setAnchorPoint(
        Vec2(
            0.5f, 0.5f
        )
    );
    _cmd_container->setPosition(
        Vec2(
             _director->getVisibleOrigin().x +  _director->getVisibleSize().width  / 2,
            (_director->getVisibleOrigin().y - 70) +  _director->getVisibleSize().height / 2
        )
    );
    addChild( _cmd_container );
    return true;
}

/* initialize menu cursor */
bool MainMenu::cursor( void )
{
    _cursor = Cursor::create(
        _cmd_container,
        { 39.5f, 37.0f },
        "../Resources/tiles/text_boxes/menu/za.png"
    );
    if ( !_cursor ) {
        return false;
    }
    _cursor->setScaleX( 0.7f );
    _cursor->setScaleY( 0.9f );
    _cursor->animate( 0.2f );
    return true;
}

void MainMenu::add_optionn( const std::string& text, const std::function<void()>& func, bool enabled, short index ) {
    _selectionz.push_back(
        std::move( std::shared_ptr<d_lan::Option>(
            new d_lan::Option(
                _cmd_container, text, func, enabled, index
            )
        ))
    ); update();
}

void MainMenu::update( void ) {
    auto cmd_size = _cmd_container->getContentSize();
    short count = _selectionz.size();
    for ( auto& selection : _selectionz ) {
        selection->getLayer()->setPosition( Vec2( cmd_size.width / 2, 12.3f * count ) );
        count--;
    }
}

void MainMenu::init_sounds( void )
{
    _sounds["cursor-move"]   = Util::get_sounds()["cursor-mv"];
    _sounds["cursor-select"] = Util::get_sounds()["cursor-sl"];
}