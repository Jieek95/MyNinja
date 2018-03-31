//
//  GameOver.h
//  My_ninja
//
//  Created by 戴杰 on 3/30/18.
//

#ifndef GameOver_h
#define GameOver_h
#include "cocos2d.h"

class GameOver : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameOver);
};

#endif /* GameOver_h */
