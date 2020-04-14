#include "Panel.h"

namespace d_lan {
    namespace Dialogue {
        Panel * Panel::create( Node *node, std::string type, std::string bg, const Vec2& coords, short z_index ) {
            Panel *panel = new Panel();
            if ( panel && panel->init( type, bg, coords ) ) {
                panel->autorelease();
                node->addChild( this, z_index );
                return panel;
            }
            return nullptr;
            CC_SAFE_DELETE( panel );
        }

        bool Panel::init( std::string type, std::string bg, const Vec2& coords ) {
            auto origin = Director::getInstance()->getVisibleOrigin();
            setAnchorPoint(
                Vec2(0.5f, 0.5f)
            );
            setPosition(
                Vec2(origin.x + coords.x, origin.y + coords.y)
            );
            set_type( type );
            set_background( bg, 255.0f );
        }

        void Panel::set_type( std::string type ) {
            _type = type;
        }
    }
}