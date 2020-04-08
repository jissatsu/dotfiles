#include "SelectionNode.h"

namespace d_lan {

    SelectionNode::SelectionNode( Node *node, const Vec2& coords, short z_index )
        : _layer(Layer::create()) {
          auto origin = Director::getInstance()->getVisibleOrigin();
          _layer->setAnchorPoint(Vec2(0.5f, 0.5f));
          _layer->setPosition(Vec2(origin.x + coords.x, origin.y + coords.y));
          node->addChild( _layer, z_index );
    }

    SelectionNode::~SelectionNode(){}
    
    void SelectionNode::set_background( std::string bg, float opacity ) {
        if ( bg == "" ) {
            _background = nullptr;
            return;
        }
        _background = Sprite::create( bg );
        _background->setAnchorPoint( Vec2(0.5f, 0.5f) );
        _background->setOpacity( opacity );
        _layer->addChild( _background );
    }

    /* Get the first enabled/disabled item */
    std::shared_ptr<d_lan::SelectionItem> SelectionNode::get( const ItemState state ) const {
        for ( auto& item : _items ) {
            if ( item->get_state() == state ) {
                return _items[item->get_index()];
            }
        }
        return nullptr;
    }

    void SelectionNode::add( std::shared_ptr<d_lan::SelectionItem> item, short z_index ) {
        _items.push_back( item );
        _layer->addChild( item->get_layer(), z_index );
    }

    Layer * SelectionNode::get_layer( void ) const {
        return _layer;
    }

    std::vector<std::shared_ptr<d_lan::SelectionItem>> SelectionNode::items( void ) const {
        return _items;
    }

    int SelectionNode::size( void ) const {
        return _items.size();
    }

}