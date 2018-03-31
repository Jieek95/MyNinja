#include "MyNinja.h"
#include "GameOver.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

enum class PhysicsCategory {
    None = 0,
    Monster = 1,    // 1
    Projectile = 2, // 2
    //All = PhysicsCategory::Monster | PhysicsCategory::Projectile // 3
    Player = 3
};

Scene* MyNinja::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0,0)); //which disables gravity
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
      // set as DEBUGDRAW_ALL then all bodys will be drawed into red
    auto layer = MyNinja::create();
    scene->addChild(layer);
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// everything are done in the "initalization"
bool MyNinja::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    // 2 initalize the origin point and the size
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto winSize = Director::getInstance()->getVisibleSize();
//    auto body = PhysicsBody::createEdgeBox(winSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
//    auto edgeNode = Node::create();
//    edgeNode->setPosition(Point(winSize.width/2,winSize.height/2));
//    edgeNode->setPhysicsBody(body);
//    this->addChild(edgeNode);
    
    // 3 set the backgroud
    auto background = DrawNode::create();
    background->drawSolidRect(origin, winSize, Color4F(0.6,0.6,0.6,1.0));
    this->addChild(background);
    
    _scoreLabel = Label::createWithSystemFont ("Score: ", "Arial", 32);
    _scoreLabel->setPosition(50, winSize.height*0.98);
    this->addChild(_scoreLabel);
    
    _score = Label::createWithSystemFont ("00", "Arial", 32);
    auto p = _scoreLabel->getPosition();
    _score->setPosition(p.x + 80, winSize.height*0.98);
    this->addChild(_score);
 
    _scoreValue = 0;
    
    // 4 add the player
    addPlayer();
    
    // 5 add the monsters
    
    srand((unsigned int)time(nullptr));
    this->schedule(schedule_selector(MyNinja::addMonster), 1.0);// 1.0s is the interval between 2 monsters
    
    // 6 add the weapon
    auto attack_Listener = EventListenerTouchOneByOne::create();
    attack_Listener->onTouchBegan = CC_CALLBACK_2(MyNinja::onTouchBegan, this);
    //pass the touch and event to MyNinja::onTouchBegan, in the MyNinja scene
           //#define CC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__,
           //std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(attack_Listener, _player);
    // get the same priority with _player
    
    // 7 move the ninja
    auto move_Listener = EventListenerKeyboard::create();
    move_Listener->onKeyPressed = CC_CALLBACK_2(MyNinja::KeyPressed, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(move_Listener, _player);
    
    // 8 collision detection
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(MyNinja::onContactBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    

    return true;
}

void MyNinja::addPlayer()
{
    auto winSize = Director::getInstance()->getVisibleSize();
    _player = Sprite::create("ninja.png");
    _player->setPosition(Vec2(winSize.width * 0.1, winSize.height * 0.5));
    auto player_Size = _player->getContentSize();
    auto physicsBody = PhysicsBody::createBox(Size(player_Size.width , player_Size.height));
    
    physicsBody->setDynamic(true);
    physicsBody->setCategoryBitmask((int)PhysicsCategory::Player);
    physicsBody->setCollisionBitmask((int)PhysicsCategory::None);
    physicsBody->setContactTestBitmask((int)PhysicsCategory::Monster);
    // not absolutely necessary
    _player->setPhysicsBody(physicsBody);
    _player->setTag(1);
    
    this->addChild(_player);
    
}

// add the monsters
void MyNinja::addMonster(float dt)
{
    auto monster = Sprite::create("enemy.png");
    
    auto monsterSize = monster->getContentSize();
    auto physicsBody = PhysicsBody::createBox(Size(monsterSize.width , monsterSize.height));
      //use a rectangle of the same size as the sprite as a decent approximation for the monster
    // 2
    physicsBody->setDynamic(true);
      // the physics engine will not apply forces to the monster
    
    // 3
    physicsBody->setCategoryBitmask((int)PhysicsCategory::Monster);
      //defines the object's type
    physicsBody->setCollisionBitmask((int)PhysicsCategory::None);
    physicsBody->setContactTestBitmask((int)PhysicsCategory::Projectile);
      //defines the object types with which collisions should generate notifications
    
    monster->setPhysicsBody(physicsBody);

    
    // 1 the rang of monsters
   
    auto winSize = Director::getInstance()->getVisibleSize();
    int minY = winSize.height * 1/5 + monsterSize.height/2;
    int maxY = winSize.height * 4/5 - monsterSize.height/2;
    int rangeY = maxY - minY;
    int randomY = (rand() % rangeY) + minY;
    
    monster->setPosition(Vec2(winSize.width + monsterSize.width/2, randomY));
    this->addChild(monster);
    
    // 2 the speed of monsters
    int minDuration = 2.0;
    int maxDuration = 4.0;
    int rangeDuration = maxDuration - minDuration;
    int randomDuration = (rand() % rangeDuration) + minDuration;
    
    // 3 create the moving monsters
    auto actionMove = MoveTo::create(randomDuration, Vec2(-monsterSize.width/2, winSize.height - randomY));
    auto actionRemove = RemoveSelf::create();
    monster->runAction(Sequence::create(actionMove,actionRemove, nullptr));
}


// get the weapon
bool MyNinja::onTouchBegan(Touch *touch, Event *unused_event)
{
    //the event is not used,we only use the touch location

    // 2
    Vec2 touchLocation = touch->getLocation();
    Vec2 offset = touchLocation - _player->getPosition();
    
    // 3 the true ninja never looks back!
    if (offset.x < 0) {
        return true;
    }
    
    // 4 create the projectile starting from the ninja
    auto projectile = Sprite::create("CloseSelected.png");
    projectile->setPosition(_player->getPosition());
    this->addChild(projectile);
    projectile->setTag(2);
    
    // 8 physical engine
    auto projectileSize = projectile->getContentSize();
    auto physicsBody = PhysicsBody::createCircle(projectileSize.width/2 );
      // uses a circle to define the physics body
    physicsBody->setDynamic(true);
    physicsBody->setCategoryBitmask((int)PhysicsCategory::Projectile);
    physicsBody->setCollisionBitmask((int)PhysicsCategory::None);
    physicsBody->setContactTestBitmask((int)PhysicsCategory::Monster);
      // not absolutely necessary
    projectile->setPhysicsBody(physicsBody);
    
    // 5 shootAmount will be long enough to exceede the window
    offset.normalize();
    auto shootAmount = offset * 1000;
    
    // 6
    auto realDest = shootAmount + projectile->getPosition();
    
    // 7
    auto actionMove = MoveTo::create(2.0f, realDest);
    auto actionRemove = RemoveSelf::create();
    projectile->runAction(Sequence::create(actionMove,actionRemove,nullptr));
    
    
    return true;
}

// move the ninja
void MyNinja::KeyPressed(EventKeyboard::KeyCode keyCode , Event* event)
{
    auto actionUp = MoveBy::create(0.2f, Vec2(0,40));
    auto actionLeft = MoveBy::create(0.2f, Vec2(-40,0));
    auto actionDown = MoveBy::create(0.2f, Vec2(0,-40));
    auto actionRight = MoveBy::create(0.2f, Vec2(40,0));
    
    Vec2 location = _player->getPosition();
    int w1 = _player->getContentSize().width;
    int h1 = _player->getContentSize().height;
    int w2 = this->getContentSize().width;
    int h2 = this->getContentSize().height;

        switch(keyCode)
        {
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case EventKeyboard::KeyCode::KEY_A:
                if(location.x > w1/2)
                    _player->runAction(actionLeft);
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            case EventKeyboard::KeyCode::KEY_D:
                if(location.x < w2 - w1/2)
                    _player->runAction(actionRight);
                break;
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
            case EventKeyboard::KeyCode::KEY_W:
                if(location.y < h2 - h1/2)
                    _player->runAction(actionUp);
                break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            case EventKeyboard::KeyCode::KEY_S:
                if(location.y > h1/2)
                    _player->runAction(actionDown);
                break;
        }
//    }
}

// collision
bool MyNinja::onContactBegan(PhysicsContact &contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    
    if(nodeA->getTag()== 1 || nodeB->getTag()== 1)
    {
        auto transition = TransitionRotoZoom::create(2.0, GameOver::createScene());
        Director::getInstance()->replaceScene(transition);
    }
    if(nodeA->getTag()== 2 || nodeB->getTag()== 2)
    {
         _score->setString(StringUtils::format("%d",_scoreValue += 50));
    }
    
    nodeA->removeFromParent();
    nodeB->removeFromParent();

    return true;
}



void MyNinja::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
