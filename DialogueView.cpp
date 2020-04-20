#include "DialogueView.h"

namespace d_lan {

    DialogueView * DialogueView::create( Node *parent, std::string bg, const Vec2& coords, float opacity ) {
        DialogueView *dialogue_view = new DialogueView();
        if ( dialogue_view && dialogue_view->init_base( parent, coords ) ) {
            dialogue_view->set_background( bg, opacity );
            dialogue_view->autorelease();
            return dialogue_view;
        }
        CC_SAFE_DELETE( dialogue_view );
        return nullptr;
    }

    void DialogueView::label( std::string name, std::string text, std::string font, float size, const Vec2& coords, float w, float h ) {
        auto label = Label::createWithTTF( text, font, size );
        if ( !label ) {
            log( "DialogueView::label() - ERROR\n" );
            return;
        }
        auto node_pos = convertToNodeSpace(getPosition());
        label->setName( name );
        label->setAnchorPoint(
            Vec2(
                0.5f, 0.5f
            )
        );
        label->setPosition(
            Vec2(
                node_pos.x + coords.x,
                node_pos.y + coords.y
            )
        );
        if ( w > 0.0f && h > 0.0f ) {
            label->setDimensions( w, h );
        }
        label->setOverflow(Label::Overflow::CLAMP);
        addChild( label, 2 );
    }

    void DialogueView::img( std::string name, std::string img, const Vec2& coords, short z_index ) {
        auto simg = Sprite::create( img );
        if ( !simg ) {
            return;
        }
        simg->setName( name );
        simg->setAnchorPoint( Vec2(0.5f, 0.5f) );
        simg->setPosition( Vec2(coords.x, coords.y) );
        addChild( simg, z_index );
    }
}