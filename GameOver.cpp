#include "GameOver.h"

GameOverScene * GameOverScene::create( void )
{
    GameOverScene *gm = new GameOverScene();
    if ( gm && gm->init() ) {
        gm->autorelease();
        return gm;
    }
    CC_SAFE_DELETE( gm );
    return nullptr;
}

bool GameOverScene::init_background( std::string img )
{
    this->bg = Sprite::create( img );
    this->bg->setAnchorPoint(
        Vec2(
            0.5f, 0.5f
        )
    );
    this->bg->setPosition(
        Vec2(
            this->origin.x + this->vsize.width  / 2,
            this->origin.y + this->vsize.height / 2
        )
    );
    this->addChild( this->bg );
    return true;
}

bool GameOverScene::init( void )
{
    if ( !Scene::init() ) {
        return false;
    }
    this->origin = Director::getInstance()->getVisibleOrigin();
    this->vsize  = Director::getInstance()->getVisibleSize();
    
    if ( !this->init_background( "../Resources/tiles/bcnd/Gameover.png" ) ) {
        log( "GameOverScene::init() - Background init failed!\n" );
        return false;
    }

    auto menu = PauseMenu::create();
    if ( !menu ) {
        return false;
    }
    if ( !menu->init( "menu", "pause", this ) ) {
        return false;
    }
    return true;
}

void GameOverScene::onEnterTransitionDidFinish()
{
    auto action = ScaleBy::create( 1.5f, 1.5f );
    this->bg->runAction( action );
}