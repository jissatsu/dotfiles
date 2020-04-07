#include "WindowBase.h"

void WindowBase::init( const Vec2& coords, short z_index ) {
    _layer->setAnchorPoint(
        Vec2(0.5f, 0.5f)
    );
    _layer->setPosition(
        Vec2(origin.x + coords.x, origin.y + coords.y)
    );
    _parent->addChild( this, z_index );
}

void WindowBase::set_background( std::string bg, float opacity ) {
    if ( bg == "" ) {
        _background = nullptr;
        return;
    }
    _background = Sprite::create( bg );
    _background->setAnchorPoint( Vec2(0.5f, 0.5f) );
    _background->setOpacity( opacity );
    _layer->addChild( _background );
}

/* Remove the window from its parent */
void WindowBase::hide( const std::function<void()>& callback ) {
    if ( getParent() != nullptr ) {
        removeFromParent();
    }
    if ( callback ) { callback(); }
}

/* Add the window to its parent */
void WindowBase::show( const std::function<void()>& callback ) {
    if ( getParent() == nullptr ) {
        _parent->addChild( this, z_index );
    }
    if ( callback ) { callback(); }
}

void WindowBase::activate( void ) {
    _isActive = true;
}

void WindowBase::deactivate( void ) {
    _isActive = false;
}