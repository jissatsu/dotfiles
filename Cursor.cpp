#include "Cursor.h"

Cursor::Cursor( Node *node, const Vec2& coords, float scaleX, float scaleY, const std::string& img )
    :_layer(Layer::create()),
     _img(Sprite::create(img)) {
    
     auto origin = Director::getInstance()->getVisibleOrigin();
     _layer->setAnchorPoint(Vec2(0.5f, 0.5f));
     _layer->addChild(_img);
     _layer->setPosition(Vec2(origin.x + coords.x, origin.y + coords.y));
     
     _img->setAnchorPoint(Vec2(0.5f, 0.5f));
     if ( scaleX >= 0.0f ) {
         _img->setScaleX( scaleX );
     }
     if ( scaleY >= 0.0f ) {
        _img->setScaleY( scaleY );
     }
     node->addChild(_layer);
}

Cursor::~Cursor(){}

Layer * Cursor::GetLayer( void ) const {
    return _layer;
}

void Cursor::pulse( float tm )
{
    Vector<FiniteTimeAction*> actions;
    actions.pushBack( FadeTo::create( tm, 200 ) );
    actions.pushBack( FadeTo::create( tm, 110 ) );
    _img->runAction( RepeatForever::create( Sequence::create( actions ) ) );
}