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
#include "DataStorageDelegate.h"
#include "ResetGameDelegate.h"
#include "AppDelegate.h"

class GameScene : public cocos2d::Scene, public DataStorageDelegate, public ResetGameDelegate {
public:
    virtual bool init();
    CREATE_FUNC(GameScene);
    
    virtual bool readData();
    virtual bool writeData(bool isGameOver);
    
    virtual void resetGame();
};

#endif /* GameScene_hpp */
