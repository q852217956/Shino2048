//
//  NumberTube.cpp
//  2048
//
//  Created by shino on 16/4/7.
//
//

#include "NumberTube.hpp"

USING_NS_CC;

NumberTube* NumberTube::create(cocos2d::Size tubeSize, cocos2d::Vec2 tubePosition) {
    auto tmpNumberTube = new NumberTube();
    if (tmpNumberTube -> init(tubeSize, tubePosition)) {
        tmpNumberTube -> autorelease();
        return tmpNumberTube;
    } else {
        delete tmpNumberTube;
        tmpNumberTube = nullptr;
        return nullptr;
    }
}

bool NumberTube::init(Size tubeSize, Vec2 tubePosition) {
    if (!Sprite::init()) {
        return false;
    }
    
    //NumberTube自身布局
    setContentSize(tubeSize);
    setPosition(tubePosition);
    
    //添加背景
    background = LayerColor::create(Color4B(255, 100, 100, 255));
    background -> setContentSize(tubeSize);
    addChild(background);
    
    //添加数字label
    numLabel = Label::createWithSystemFont("0", "Arial", 50);
    numLabel -> setTextColor(Color4B(255, 255, 255, 255));
    numLabel -> setPosition(Vec2(tubeSize.width / 2, tubeSize.height / 2));
    setNum(0);
    addChild(numLabel);
    return true;
}

void NumberTube::setUpdateDelegator(ScoreDelegate *delegator){
    updateDelegator = delegator;
}

int NumberTube::getNum() {
    return num;
}

bool NumberTube::setNum(int value) {
    num = value;
    numLabel -> setString(std::to_string(value));
    if (value == 0) {
        setVisible(false);
    } else {
        setVisible(true);
    }
    switch (value) {
        case 0:
            background -> setColor(Color3B(240, 230, 224));
            break;
        case 2:
            background -> setColor(Color3B(0, 224, 224));
            break;
        case 4:
            background -> setColor(Color3B(0, 206, 224));
            break;
        case 8:
            background -> setColor(Color3B(0, 179, 224));
            break;
        case 16:
            background -> setColor(Color3B(0, 152, 224));
            break;
        case 32:
            background -> setColor(Color3B(0, 125, 224));
            break;
        case 64:
            background -> setColor(Color3B(0, 98, 224));
            break;
        case 128:
            background -> setColor(Color3B(0, 62, 224));
            break;
        case 256:
            background -> setColor(Color3B(0, 8, 224));
            break;
        case 512:
            background -> setColor(Color3B(89, 8, 224));
            break;
        case 1024:
            background -> setColor(Color3B(125, 8, 224));
            break;
        case 2048:
            background -> setColor(Color3B(168, 8, 224));
            break;
        case 4096:
            background -> setColor(Color3B(197, 8, 224));
            break;
        case 8192:
            background -> setColor(Color3B(233, 8, 224));
            break;
        case 16384:
            background -> setColor(Color3B(233, 8, 146));
            break;
        case 32768:
            background -> setColor(Color3B(233, 8, 111));
            break;
        case 65536:
            background -> setColor(Color3B(233, 8, 83));
            break;
        case 131072:
            background -> setColor(Color3B(233, 8, 43));
            break;
        default:
            return false;
    }
        return true;
}

bool NumberTube::setRandomNum() {
    int tmpNum;
    if (CCRANDOM_0_1() * 10 < 1) {
        tmpNum = 4;
    } else {
        tmpNum = 2;
    }
    setNum(tmpNum);
    return true;
}

void NumberTube::runActionMoveTo(NumberTube *tube) {
    auto move = MoveTo::create(0.04 * tube -> getPosition().distance(getPosition()) / (getContentSize().width + 40), tube -> getPosition());
    auto delay = DelayTime::create(0.1);
    auto callFunc = CallFunc::create([=]{
        auto tmpNum = getNum();
        setNum(0);
        tube -> setNum(tmpNum);
    });
    auto place = Place::create(getPosition());
    auto seq = Sequence::create(move, delay, callFunc, place, NULL);
    this -> runAction(seq);
}

void NumberTube::runActionAddTo(NumberTube *tube) {
    auto move = MoveTo::create(0.04 * tube -> getPosition().distance(getPosition()) / (getContentSize().width + 40), tube -> getPosition());
    auto delay = DelayTime::create(0.1);
    auto callFunc = CallFunc::create([=] {
        setNum(0);
    });
    auto place = Place::create(getPosition());
    auto seq = Sequence::create(move, delay, callFunc, place, NULL);
    this -> runAction(seq);
}

void NumberTube::runActionAddBy(NumberTube *tube) {
    auto move = MoveTo::create(0.04 * tube -> getPosition().distance(getPosition()) / (getContentSize().width + 40), tube -> getPosition());
    auto scale1 = ScaleTo::create(0.05, 1.3);
    auto scale2 = ScaleTo::create(0.05, 1);
    auto callFunc = CallFunc::create([=]{
        int tmpNum = getNum() * 2;
        updateDelegator -> updateScore(tmpNum);
        setNum(0);
        tube -> setNum(tmpNum);
    });
    auto place = Place::create(getPosition());
    auto seq = Sequence::create(move, scale1, scale2, callFunc, place, NULL);
    this -> runAction(seq);
}