#include "MainMenuScene.h"
#include "HotelScene1.h"
//#include "GameOver.h"

MainMenuScene * MainMenuScene::create( CallbackManager *manager )
{
    MainMenuScene *scene = new MainMenuScene();
    if ( scene && scene->init( manager ) ) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE( scene );
    return nullptr;
}

bool MainMenuScene::init_background( std::string img, float z_index )
{
    this->bg = Sprite::create( img );
    if (this->bg == nullptr) {
        return false;
    }
    this->bg->setAnchorPoint(
        Vec2(
            0.5f, 0.5f
        )
    );
    this->bg->setPosition(
        Vec2(
            this->origin.x + this->vsize.width  / 2,
            this->origin.y - 5 + this->vsize.height / 2
        )
    );
    this->addChild( this->bg, z_index );
    return true;
}

bool MainMenuScene::init_menu( void )
{
    this->menu = MainMenu::create();
    if ( !this->menu ) {
        return false;
    }
    this->menu->init_callbacks = [=]( void )
    {
        this->menu->add_callback( "New Game", CC_CALLBACK_0( MainMenuScene::NewGame, this ) );
        this->menu->add_callback( "Load",     CC_CALLBACK_0( MainMenuScene::NewGame, this ) );
        this->menu->add_callback( "End",      CC_CALLBACK_0( MainMenuScene::Die, this ) );
    };
    if ( !this->menu->init( "menu", "main", this ) ) {
        return false;
    }
    this->menu->show();
    return true;
}

bool MainMenuScene::init( CallbackManager *manager )
{
    if ( !Scene::init() ) {
        return false;
    }
    // glfwSwapInterval( 0 );
    this->vsize  = Director::getInstance()->getVisibleSize();
    this->origin = Director::getInstance()->getVisibleOrigin();

    if ( !this->init_background( "../Resources/tiles/bcnd/nagatoo.png", 0 ) ) {
        return false;
    }
    if ( !this->init_menu() ) {
        return false;
    }
    init_keyboard( this, this->_eventDispatcher );
    this->MainTrackId = AudioEngine::play2d(
        Util::get_sounds()["main-menu"],
        true,
        0.8f
    );
    return true;
}

void MainMenuScene::init_keyboard( Scene *scene, EventDispatcher *disp )
{
    auto listn = EventListenerKeyboard::create();
    listn->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        switch ( keyCode ) {
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
                this->menu->prev();
                break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                this->menu->next();
                break;
            case EventKeyboard::KeyCode::KEY_ENTER:
                this->menu->select();
                break;
        }
    };
    disp->addEventListenerWithSceneGraphPriority( listn, scene );
}

void MainMenuScene::NewGame( void )
{
    this->menu->hide();
    AudioEngine::pause( this->MainTrackId );
    Director::getInstance()->replaceScene(
        TransitionFade::create(1.7f, HotelScene1::create(), Color3B( 0, 0, 0 ))
    );
}

void MainMenuScene::Die( void )
{
    Util::clear_all();
    this->menu->_callback_manager->release();
    Director::getInstance()->end();
}