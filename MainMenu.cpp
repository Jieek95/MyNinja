//
//  MainMenu.cpp
//  My_ninja
//
//  Created by 戴杰 on 3/30/18.
//

#include "MainMenu.h"
#include "MyNinja.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

Scene* MainMenu::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenu::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenu::init()
{
    if(!Layer::init())
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto l = Label::createWithSystemFont("True Ninja", "Arial", 100);
    l->setPosition(visibleSize.width / 2, visibleSize.height* 0.7);
    this->addChild(l);
    
    // "start game" button
    auto start_button = Button::create("button.png");
    start_button->setScale(0.1);
    start_button->setTitleText("start Game");
    start_button->setTitleFontName("Arial");
    start_button->setTitleFontSize(400);
    start_button->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.5));
    start_button->addTouchEventListener([](Ref* pSender,Widget::TouchEventType type){
        if(type == Widget::TouchEventType::ENDED){
            //
            auto transition = TransitionRotoZoom::create(1.0, MyNinja::createScene());
            Director::getInstance()->replaceScene(transition);
        }
    });
    this->addChild(start_button);
  
    // "exit" button
    auto close_button = Button::create("button.png");
    close_button->setScale(0.1);
    close_button->setTitleText("exit");
    close_button->setTitleFontName("Arial");
    close_button->setTitleFontSize(400);
    close_button->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.3));
    close_button->addTouchEventListener([](Ref* pSender,Widget::TouchEventType type)
    {
        Director::getInstance()->end();
        
       #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
       #endif
    });
    this->addChild(close_button);

    return true;
}
