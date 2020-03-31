#include "OptionListView.h"

OptionListView::OptionListView( Node *parent, const std::string& bg, const Vec2& coords )
    :_layer(Layer::create()),
     _background(Sprite::create(bg)) {
    
     auto origin = Director::getInstance()->getVisibleOrigin();
     _layer->setAnchorPoint(Vec2(0.5f, 0.5f));
     _layer->addChild(_background);
     _layer->setPosition(Vec2(origin.x + coords.x, origin.y + coords.y));
     _background->setAnchorPoint(Vec2(0.5f, 0.5f));
     parent->addChild(_layer);
}

OptionListView::~OptionListView(){}

Layer * OptionListView::GetLayer( void ) const {
    return _layer;
}
