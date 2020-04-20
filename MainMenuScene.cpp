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

bool MainMenuScene::init_background( void )
{
    bg = Sprite::create( UI::MainMenu::bg );
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
    addChild( bg, 0 );
    return true;
}

bool MainMenuScene::init_menu( void )
{
    menu = MainMenu::create( this );
    if ( !menu ) {
        return false;
    }
    auto node = menu->selection_node(0);
    menu->add_option( node, "New Game", CC_CALLBACK_0( MainMenuScene::NewGame, this ), true,  0 );
    menu->add_option( node, "Load",     CC_CALLBACK_0( MainMenuScene::NewGame, this ), false, 1 );
    menu->add_option( node, "End",      CC_CALLBACK_0( MainMenuScene::Die, this ), true, 2 );

    menu->show([=](void){
        auto action = MoveBy::create( 0.1f, {0.0f, 65.0f} );
        menu->runAction( action );
    }, 1);
    return true;
}

void MainMenuScene::init_dialogue_manager( void )
{
    _dialogue_manager = d_lan::DialogueManager::instance();
    _dialogue_manager->set_view(
        this,
        UI::DialogueView::bg,
        {20.0f, 20.0f},
        100.0f
    );
    auto view = _dialogue_manager->get_view();
    // character name
    view->label( "name", "", "fonts/ProzaLibre-Regular.ttf", {10.0f, 30.0f}, 0.0f, 0.0f );
    // dialogue text
    view->label( "text", "", "fonts/ProzaLibre-Regular.ttf", {10.0f, 30.0f}, 200.0f, 30.0f );
    // character expression (face)
    view->img( "expression", "", {10.0f, 0.0f}, 2 );
    _dialogue_manager->set_timer( 0.1f )->set_beep( "" );
}

bool MainMenuScene::init( void )
{
    if ( !Scene::init() ) {
        return false;
    }
    // glfwSwapInterval( 0 );
    vsize  = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    if ( !init_background() ) {
        return false;
    }
    init_dialogue_manager();
    return true;
}

void MainMenuScene::init_keyboard( Scene *scene, EventDispatcher *disp )
{
    auto listn = EventListenerKeyboard::create();
    listn->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        switch ( keyCode ) {
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
                menu->prev();
                AudioEngine::play2d(
                    AudioCFG::MainMenu::toggle
                );
                break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                menu->next();
                AudioEngine::play2d(
                    AudioCFG::MainMenu::toggle
                );
                break;
            case EventKeyboard::KeyCode::KEY_ENTER:
                menu->confirm();
                AudioEngine::play2d(
                    AudioCFG::MainMenu::select
                );
                break;
        }
    };
    disp->addEventListenerWithSceneGraphPriority( listn, scene );
}

void MainMenuScene::onEnterTransitionDidFinish()
{
    if ( !init_menu() ) {
        Die();
    }
    init_keyboard( this, _eventDispatcher );
    //_dialogue_manager->set_dialogue( "../Resources/res/dialogue/dialogue-001.json" );
    MainTrackId = AudioEngine::play2d( AudioCFG::MainMenu::menu, true, 0.8f );
}

void MainMenuScene::update( float delta ) {
    //_dialog_manager->update();
}

void MainMenuScene::NewGame( void )
{
    menu->hide([=](void){
        auto action = MoveBy::create( 0.1f, {0.0f, -90.0f} );
        menu->runAction( action );
    }, 0.4f);
    
    AudioEngine::stop( MainTrackId );
    Director::getInstance()->replaceScene(
        TransitionFade::create(3.7f, HotelScene1::create(), Color3B( 0, 0, 0 ))
    );
}

void MainMenuScene::Die( void )
{
    Util::clear_all();
    Director::getInstance()->end();
}
