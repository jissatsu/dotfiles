#include "Util.h"

std::string Util::get_file_name( std::string path )
{
    if ( path == "" ) {
        return "";
    }
    const size_t name_begin = path.find_last_of("\\/");
    if ( name_begin != std::string::npos ) {
        return path.erase(0, name_begin + 1);
    }
    return "";
}

/*
 * Get player spawn point from a tmx tilemap
 */
Vec2 Util::get_spawn_point( TMXTiledMap *tile_map )
{
    TMXObjectGroup *group = tile_map->getObjectGroup( "spawn-point" );
    ValueVector& objects = group->getObjects();
    if ( objects.size() <= 0 ) {
        return Vec2(
            0.0f, 0.0f
        );
    }
    for ( auto obj : objects ) {
        ValueMap& dict = obj.asValueMap();
        if ( dict["name"].asString().find( "spawn" ) != std::string::npos ) {
            float x = dict["x"].asFloat();
            float y = dict["y"].asFloat();
            return Vec2( x, y );
        }
    }
    return Vec2( 0.0f, 0.0f );
}

TMXLayer * Util::get_collision_layer( TMXTiledMap *tile_map )
{
    TMXLayer *layer = tile_map->getLayer( "collision" );
}

void Util::clear_all( void )
{
    AudioEngine::uncacheAll();
    AudioEngine::end();
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache::destroyInstance();
}

std::map<std::string, const char *> Util::get_sounds( void )
{
    std::map<std::string, const char *> sounds;
    sounds["main-menu"] = "../Resources/sounds/Jasper Byrne - Lone Survivr-Whispers.mp3";
    sounds["cursor-mv"] = "../Resources/sounds/zipclick.flac";
    sounds["cursor-sl"] = "../Resources/sounds/click.wav";
    return sounds;
}