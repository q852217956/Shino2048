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
#include "array"
#include "cocos2d.h"
#include "NumberTube.hpp"
#include "ScoreDelegate.h"
#include "ResetGameDelegate.h"
#include "ScoreScene.hpp"

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class TubeLayer : public cocos2d::Layer, public ScoreDelegate {
public:
    
    virtual bool init() override;
    CREATE_FUNC(TubeLayer);
    
    virtual void updateScore(int num) override;
    virtual void updateActionNum() override;
    
    bool readData();
    bool writeData(bool isGameOver);
    
    void setNewGame(ResetGameDelegate *newGameScene);
    
private:
    
    NumberTube *tube[4][4];
    std::array<std::array<NumberTube*, 4>, 4> arrangeArray(Direction direction);
    void move(Direction direction);
    
    void touchInit(cocos2d::Rect touchArea);
    
    void labelInit(cocos2d::Rect labelArea);
    void tubeInit(cocos2d::Rect tubeArea);
    
    void randomTubeNum(float delayTime);
    void update(float dt) override;
    
    int actionNum;
    
    ResetGameDelegate *newScene;
    
    bool isTouchMoved;
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    
    cocos2d::Label *scoreLabel;
    cocos2d::Label *highestScoreLabel;
    cocos2d::Label *message;
    NumberTube *highestTube;
    
    int score;
    
    std::vector<Direction> touchMoveStack;
};

#endif /* TubeLayer_hpp */
