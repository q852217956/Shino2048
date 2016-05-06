//
//  NumberTube.hpp
//  2048
//
//  Created by shino on 16/4/7.
//
//

#ifndef NumberTube_hpp
#define NumberTube_hpp

#include <stdio.h>
#include "cocos2d.h"

class NumberTube : public cocos2d::Sprite {
public:
    
    bool init(cocos2d::Size tubeSize, cocos2d::Vec2 tubePosition);
    static NumberTube* create(cocos2d::Size tubeSize, cocos2d::Vec2 tubePosition);
    
    int getNum();
    bool setNum(int value);
    bool setRandomNum();
    
    //动作
    cocos2d::Sequence *actionMoveTo(NumberTube *tube);
    cocos2d::Sequence *actionAddBy(NumberTube *tube);
    cocos2d::Sequence *actionAddTo(NumberTube *tube);
    
    void action(NumberTube *tube, int num);
    
private:
    
    int num;
    cocos2d::LayerColor *background;
    cocos2d::Label *numLabel;
    
    void nullFunc();
};

#endif /* NumberTube_hpp */
