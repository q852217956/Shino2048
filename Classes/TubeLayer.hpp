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
#include "TubeDelegate.h"
#include "ResetGameDelegate.h"
#include "ScoreScene.hpp"

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class TubeLayer : public cocos2d::Layer, public TubeDelegate {
public:
    virtual bool init() override;
    CREATE_FUNC(TubeLayer);
    
    //TubeDelegate
    virtual void updateScore(int num) override;
    virtual void updateActionNum() override;
    virtual void updateIsFinished() override;
    
    bool readData();
    bool writeData(bool isGameOver);
    
    void setNewGame(ResetGameDelegate *newGameScene);
    
private:
    NumberTube *tube[4][4];
    std::array<std::array<NumberTube*, 4>, 4> arrangeArray(Direction direction);
    void move(Direction direction);
    
    std::queue<Direction> touchMoveQueue;
    void touchInit(cocos2d::Rect touchArea);
    void labelInit(cocos2d::Rect labelArea);
    void tubeInit(cocos2d::Rect tubeArea);
    
    void randomTubeNum();
    void update(float dt) override;
    
    bool isTouchMoved;
    int actionNum;
    
    ResetGameDelegate *newScene;
    
    cocos2d::Label *scoreLabel;
    cocos2d::Label *highestScoreLabel;
    cocos2d::Label *message;
    NumberTube *highestTube;
    int score;
};

#endif /* TubeLayer_hpp */
