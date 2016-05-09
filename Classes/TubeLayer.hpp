//
//  TubeLayer.hpp
//  2048
//
//  Created by shino on 16/4/7.
//
//

#ifndef TubeLayer_hpp
#define TubeLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "NumberTube.hpp"
#include "ScoreDelegate.h"

class TubeLayer : public cocos2d::Layer, public ScoreDelegate {
public:
    
    virtual bool init();
    CREATE_FUNC(TubeLayer);
    
    virtual void updateScore(int num);

private:
    
    NumberTube *tube[4][4];
    
    void touchInit(cocos2d::Rect touchArea);
    void labelInit(cocos2d::Rect labelArea);
    void tubeInit(cocos2d::Rect tubeArea);
    
    void randomTubeNum(float delayTime);
    
    cocos2d::Vec2 touchPoint;
    bool isTouchMoved;
    bool isMoveFinished;
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    
    cocos2d::Label *scoreLabel;
    cocos2d::Label *highestScoreLabel;
    NumberTube *highestTube;
    
    int score;
    int highestTubeNum;
    
    std::vector<int> touchMoveStack;
};

#endif /* TubeLayer_hpp */
