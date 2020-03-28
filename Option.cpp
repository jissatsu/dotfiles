#include "Option.h"

Option::Option( const std::string& text, const Vec2& pos,
                const std::function<void()>& func, bool enabled )
    :_layer(Layer::create()),
     _label(Label::createWithTTF( text, ITEM_FONT, 8.0f )),
     _enabled(enabled) {
    
     _label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
     _layer->addChild( _label );
     _layer->setPosition( Vec2( pos.x, pos.y ) );
}

Option::~Option(){}

void Option::SetHandler( const std::function<void()>& func ) {
    auto handler = CallFunc::create( func );
    _handler = handler;
}

CallFunc * Option::GetHandler( void ) const {
    return _handler;
}

void Option::SetIndex( short index ) {
    _index = index;
}

short Option::GetIndex( void ) const {
    return _index;
}