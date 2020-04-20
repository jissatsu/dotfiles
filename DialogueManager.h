#ifndef __D_LAN_DIALOG_MANAGER_HH
#define __D_LAN_DIALOG_MANAGER_HH 1

#include <iostream>
#include "../../cocos2d/cocos/cocos2d.h"
#include "../config.h"
#include "../Utils/json/json-util.h"
#include "DialogueView.h"

USING_NS_CC;

using namespace json_util;

namespace d_lan {

    class DialogueManager {
        public:
            DialogueManager( float timer );
            ~DialogueManager();
            static std::shared_ptr<DialogueManager> instance( void );
            std::shared_ptr<DialogueManager> set_timer( float timer );
            std::shared_ptr<DialogueManager> set_beep( std::string file );
            bool set_view( Node *node, std::string bg, const Vec2& coords, float opacity );
            void set_dialogue( std::string json_file );
            void show_next( void );
            void update( float dt );
            void handleInput( void );
            d_lan::DialogueView * get_view( void ) const;

        protected:
            static std::shared_ptr<DialogueManager> _instance;
            d_lan::DialogueView *_dialogue_view;
            std::queue<std::shared_ptr<d_lan::Dialogue>> _dialogue;
            std::string _beep_sound;
            float _timer;
            bool _has_timer;
            bool _text_ended;
    };
}

#endif