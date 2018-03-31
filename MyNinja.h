#ifndef MYNINJA_H
#define MYNINJA_H

#include "cocos2d.h"

class MyNinja : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void addPlayer();
    
    void addMonster(float dt);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    void KeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
    
    bool onContactBegan(cocos2d::PhysicsContact &contact);
    
    bool Over(cocos2d::PhysicsContact &contact);

    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MyNinja);
private:
    cocos2d::Sprite* _player;
    // score label
    cocos2d::Label* _scoreLabel;
    cocos2d::Label* _score;
    // score
    int _scoreValue;
};

#endif 
