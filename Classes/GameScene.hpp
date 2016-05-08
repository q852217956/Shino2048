//
//  GameScene.hpp
//  2048
//
//  Created by shino on 16/4/7.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <stdio.h>
#include "cocos2d.h"

class GameScene : public cocos2d::Scene {
public:
    
    virtual bool init();
    CREATE_FUNC(GameScene);
    
};

#endif /* GameScene_hpp */
