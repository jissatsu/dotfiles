#include "WindowBase.h"

namespace d_lan {
    
    bool WindowBase::init_base( Node *parent, const Vec2& coords, short z_index ) {
        if ( parent == nullptr ) {
            return false;
        }
        auto origin = Director::getInstance()->getVisibleOrigin();
        setAnchorPoint(
            Vec2(0.5f, 0.5f)
        );
        setPosition(
            Vec2(origin.x + coords.x, origin.y + coords.y)
        );
        _parent = parent;
        return true;
    }

    void WindowBase::set_background( std::string bg, float opacity ) {
        if ( bg == "" ) {
            _background = nullptr;
            return;
        }
        _background = Sprite::create( bg );
        _background->setAnchorPoint( Vec2(0.5f, 0.5f) );
        _background->setOpacity( opacity );
        addChild( _background );
    }

    /* add the window to its parent */
    /* this method can be overriden */
    void WindowBase::show( const std::function<void()>& callback, short z_index ) {
        if ( !_isActive ) {
            _parent->addChild( this, z_index );
            if ( callback ) { callback(); }
        }
        _isActive = true;
    }

    /* remove from parent after `hide_delay` time */
    /* this method can be overriden */
    void WindowBase::hide( const std::function<void()>& callback, float hide_delay ) {
        if ( _isActive ) {
            if ( callback ) { callback(); }
            auto _hide = CallFunc::create([=](void){
                if ( getParent() != nullptr ) {
                    removeFromParent();
                }
            });
            runAction( Sequence::create(
                DelayTime::create(hide_delay), _hide, nullptr)
            );
        }
        _isActive = false;
    }
}