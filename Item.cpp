#include "Item.h"

namespace d_lan {
    // MenuCoreItem inherit Node

    /*MenuItem * MenuItem::create( std::string json_file )
    {
        MenuItem *item = new MenuItem();
        if ( item ) {
            Document json = json_util::json_decode( json_file );
            item->init( json );
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE( item );
        return nullptr;
    }

    void MenuItem::Init( Document& json )
    {
        _itemProfile.name.assign( json["name"].GetString() );
        _itemProfile.text.assign( json["text"].GetString() );
        _itemProfile.x = json["x"].GetFloat();
        _itemProfile.y = json["y"].GetFloat();
        _itemProfile.enabled = json["text"].GetBool();
    }*/

    void MenuItem::SetCallback( const std::function<void()> func ) {
        auto callback = CallFunc::create( func );
        _Callback = callback;
        _Callback->retain();
    }

    CallFunc * MenuItem::GetCallback( void ) const {
        return _Callback;
    }

    void MenuItem::SetEnabled( bool enabled )
    {
        _enabled = enabled;
        if ( !enabled ) {
            setTextColor( Color4B( 89, 89, 89, 255 ) );
        }
    }

    /*const std::string& Item::GetName() const {
        return _itemProfile.name;
    }*/

    const std::string MenuItem::GetState( void ) const {
        return _enabled ? "enabled" : "disabled" ;
    }
}