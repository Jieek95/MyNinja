//
//  GameOver.cpp
//  My_ninja
//
//  Created by 戴杰 on 3/30/18.
//

#include "GameOver.h"
USING_NS_CC;

Scene* GameOver::createScene()
{
    auto scene = Scene::create();
    auto layer = GameOver::create();
    scene->addChild(layer);
    return scene;
}

bool GameOver::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto l = Label::createWithSystemFont("Game Over", "Arial", 100);
    l->setPosition(visibleSize.width / 2, visibleSize.height* 0.5);
    this->addChild(l);
    
    return true;
}
