#include "SelectionItem.h"

namespace d_lan {
    
    SelectionItem::SelectionItem( Node *node, const std::string& text, const std::function<void()>& func, bool enabled, short index )
        :_layer(Layer::create()),
         _label(Label::createWithTTF( text, ITEM_FONT, 8.0f )),
         _enabled(enabled) {
        
         set_handler( func );
         set_index( index );
         if ( !enabled ) {
             _label->setTextColor( Color4B( 89, 89, 89, 255 ) );
         }
         _label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
         _layer->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
         _layer->addChild( _label );
          node->addChild( _layer, 2 );
    }

    SelectionItem::~SelectionItem() {
        _handler->release();
        log( "handler released!\n" );
    }

    void SelectionItem::set_handler( const std::function<void()>& func ) {
        auto handler = CallFunc::create( func );
        _handler = handler;
        _handler->retain();
    }

    CallFunc * SelectionItem::get_handler( void ) const {
        return _handler;
    }
    
    const std::string SelectionItem::get_state( void ) const {
        return _enabled ? "enabled" : "disabled" ;
    }

    const std::string SelectionItem::get_text( void ) const {
        return _label->getString();
    }

    void SelectionItem::set_text( const std::string& text ) {
        _label->setString( text );
    }

    void SelectionItem::set_index( short index ) {
        _index = index;
    }

    short SelectionItem::get_index( void ) const {
        return _index;
    }

    Layer * SelectionItem::get_layer( void ) const {
        return _layer;
    }
}
