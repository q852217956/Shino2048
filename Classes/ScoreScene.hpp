//
//  ScoreScene.hpp
//  2048
//
//  Created by shino on 16/4/7.
//
//

#ifndef ScoreScene_hpp
#define ScoreScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "GameScene.hpp"

class ScoreScene : public cocos2d::Scene {
public:
    
    virtual bool init();
    
    CREATE_FUNC(ScoreScene);
};

class ScoreLayer : public cocos2d::Layer {
public :
    
    virtual bool init();
    
    CREATE_FUNC(ScoreLayer);
};

#endif /* ScoreScene_hpp */
