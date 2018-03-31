//
//  MainMenu.h
//  My_ninja
//
//  Created by 戴杰 on 3/30/18.
//

#ifndef MainMenu_h
#define MainMenu_h

#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
};
#endif /* MainMenu_h */
