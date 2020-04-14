#include "DialogueView.h"

namespace d_lan {

    DialogueView * DialogueView::create( Node *node, std::string bg, const Vec2& coords, short z_index ) {
        DialogueView *dialogue_view = new DialogueView();
        if ( dialogue_view ) {
            dialogue_view->set_background( bg, 255.0f );
            dialogue_view->autorelease();
            return dialogue_view;
        }
        CC_SAFE_DELETE( dialogue_view );
        return nullptr;
    }

    void DialogueView::add_panel( std::string type, std::string bg, const Vec2& coords, short z_index ) {
        if ( !profile.get() ) {
            throw ("DialogueView::Panel - type not specified!\n");
        }
        auto panel = d_lan::Dialogue::Panel::create(this, type, bg, coords, z_index);
        _panels.push_back( panel );
    }

    d_lan::Dialogue::Panel DialogueView::get_panel( std::string type ) {
        for ( auto panel& : _panels ) {
            if ( panel->get_type() == type ) {
                return panel;
            }
        } return nullptr;
    }

    /* set character portrait to be displayed */
    void DialogueView::set_portrait( std::string img, const Vec2& coords, short z_index ) {
        _portrait = Sprite::create( img );
        if ( !_portrait ) {
            throw ("Couldn't find DialogueView portrait img!\n");
        }
        _portrait->setAnchorPoint(Vec2(0.5f, 0.5f));
        _portrait->setPosition(Vec2(coords.x, coords.y));
        addChild( _portrait, z_index );
    }

    /* set character name to be displayed */
    void DialogueView::set_name( std::string name, std::string font, float size, short z_index ) {
        std::string _font = "";
        if ( font.empty() ) {
            _font = std::string(ITEM_FONT);
        } else {
            _font = font;
        }
        _label = Label::createWithTTF( name, _font, size );
        _label->setAnchorPoint(Vec2(0.5f, 0.5f));
        _label->setPosition(Vec2(coords.x, coords.y));
        addChild( _label, z_index );
    }
}