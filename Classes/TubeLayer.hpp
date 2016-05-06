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

class TubeLayer : public cocos2d::Layer {
public:
    virtual bool init();
    
    CREATE_FUNC(TubeLayer);
private:
    
    NumberTube *tube[4][4];
    
    void randomTubeNum(float delayTime);
    
    cocos2d::Vec2 touchPoint;
    bool isTouchMoved;
    bool isMoveFinished;
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    
    cocos2d::Vec2 setTubePosition(cocos2d::Vec2 tubeCoordinate);
    
    std::vector<int> touchMoveStack;
};

#endif /* TubeLayer_hpp */
