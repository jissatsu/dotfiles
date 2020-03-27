#include "MainMenuScene.h"
#include "HotelScene1.h"
//#include "GameOver.h"

MainMenuScene * MainMenuScene::create( void )
{
    MainMenuScene *scene = new MainMenuScene();
    if ( scene && scene->init() ) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE( scene );
    return nullptr;
}

bool MainMenuScene::init_background( std::string img, float z_index )
{
    bg = Sprite::create( img );
    if (bg == nullptr) {
        return false;
    }
    bg->setAnchorPoint(
        Vec2(
            0.5f, 0.5f
        )
    );
    bg->setPosition(
        Vec2(
            origin.x + vsize.width  / 2,
            origin.y - 5 + vsize.height / 2
        )
    );
    addChild( bg, z_index );
    return true;
}

bool MainMenuScene::init_menu( void )
{
    menu = MainMenu::create();
    if ( !menu ) {
        return false;
    }
    menu->init_callbacks = [=]( void )
    {
        menu->add_callback( "New Game", CC_CALLBACK_0( MainMenuScene::NewGame, this ) );
        menu->add_callback( "Load",     CC_CALLBACK_0( MainMenuScene::NewGame, this ) );
        menu->add_callback( "End",      CC_CALLBACK_0( MainMenuScene::Die, this ) );
    };
    if ( !menu->init( "menu", "main", this ) ) {
        return false;
    }
    menu->show();
    return true;
}

bool MainMenuScene::init( void )
{
    if ( !Scene::init() ) {
        return false;
    }
    // glfwSwapInterval( 0 );
    vsize  = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    if ( !init_background( "../Resources/tiles/bcnd/nagatoo.png", 0 ) ) {
        return false;
    }
    return true;
}

void MainMenuScene::onEnterTransitionDidFinish()
{
    if ( !init_menu() ) {
        Die();
    }
    //init_keyboard( this, _eventDispatcher );
    menu->retain();
    _input = std::unique_ptr<Input>( new Input( this ) );
    if ( _input != nullptr ) {
        _input->activate();
    }
    scheduleUpdate();
    MainTrackId = AudioEngine::play2d( Util::get_sounds()["main-menu"], true, 0.8f );
}

void MainMenuScene::NewGame( void )
{
    menu->hide();
    AudioEngine::pause( MainTrackId );
    Director::getInstance()->replaceScene(
        TransitionFade::create(1.7f, HotelScene1::create(), Color3B( 0, 0, 0 ))
    );
}

void MainMenuScene::Die( void )
{
    Util::clear_all();
    Director::getInstance()->end();
}

void MainMenuScene::update( float dt )
{
    if ( _input->isKeyPressed( EventKeyboard::KeyCode::KEY_UP_ARROW ) ) {   menu->prev(); return; }
    if ( _input->isKeyPressed( EventKeyboard::KeyCode::KEY_DOWN_ARROW ) ) { menu->next(); return; }
    if ( _input->isKeyPressed( EventKeyboard::KeyCode::KEY_ENTER ) ) {
        _input->deactivate();
        menu->select();
        return;
    }
}