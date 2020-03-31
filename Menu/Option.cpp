#include "Option.h"

namespace d_lan {
    
    Option::Option( Node *node, const std::string& text, const std::function<void()>& func, bool enabled, short index )
        :_layer(Layer::create()),
         _label(Label::createWithTTF( text, ITEM_FONT, 8.0f )),
         _enabled(enabled) {
        
         SetHandler( func );
         SetIndex( index );
         if ( !enabled ) {
             _label->setTextColor( Color4B( 89, 89, 89, 255 ) );
         }
         _label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
         _layer->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
         _layer->addChild( _label );
          node->addChild( _layer, 2 );
    }

    Option::~Option(){}

    void Option::SetHandler( const std::function<void()>& func ) {
        auto handler = CallFunc::create( func );
        _handler = handler;
        _handler->retain();
    }

    CallFunc * Option::GetHandler( void ) const {
        return _handler;
    }
    
    const std::string Option::GetState( void ) const {
        return _enabled ? "enabled" : "disabled" ;
    }

    void Option::SetIndex( short index ) {
        _index = index;
    }

    short Option::GetIndex( void ) const {
        return _index;
    }

    Layer * Option::getLayer( void ) const {
        return _layer;
    }
}