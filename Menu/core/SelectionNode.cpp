#include "SelectionNode.h"

namespace d_lan {

    SelectionNode::SelectionNode( Node *node, std::string bg, const Vec2& coords )
        : _layer(Layer::create()),
          _background(Sprite::create(bg)) {
          auto origin = Director::getInstance()->getVisibleOrigin();
          _background->setAnchorPoint(Vec2(0.5f, 0.5f));
          _layer->setAnchorPoint(Vec2(0.5f, 0.5f));
          _layer->setPosition(Vec2(origin.x + coords.x, origin.y + coords.y));
          _layer->addChild( _background );
          node->addChild( _layer, 2 );
    }
    SelectionNode::~SelectionNode(){}

    void SelectionNode::add( std::shared_ptr<d_lan::SelectionItem> item ) {
        _items.push_back( item );
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