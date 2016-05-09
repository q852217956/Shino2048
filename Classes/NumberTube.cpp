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
    switch (value) {
        case 0:
            setVisible(false);
            break;
        case 2:
            background -> setColor(Color3B(250, 232, 225));
            setVisible(true);
            break;
        case 4:
            background -> setColor(Color3B(250, 229, 204));
            setVisible(true);
            break;
        case 8:
            background -> setColor(Color3B(250, 180, 130));
            setVisible(true);
            break;
        case 16:
            background -> setColor(Color3B(236, 142, 92));
            setVisible(true);
            break;
        case 32:
            background -> setColor(Color3B(244, 124, 100));
            setVisible(true);
            break;
        case 64:
            background -> setColor(Color3B(250, 92, 73));
            setVisible(true);
            break;
        case 128:
            background -> setColor(Color3B(251, 209, 125));
            setVisible(true);
            break;
        case 256:
            background -> setColor(Color3B(249, 107, 111));
            setVisible(true);
            break;
        case 512:
            background -> setColor(Color3B(248, 202, 99));
            setVisible(true);
            break;
        case 1024:
            background -> setColor(Color3B(227, 185, 48));
            setVisible(true);
            break;
        case 2048:
            background -> setColor(Color3B(248, 195, 64));
            setVisible(true);
            break;
        case 4096:
            background -> setColor(Color3B(243, 109, 115));
            setVisible(true);
            break;
        case 8192:
            background -> setColor(Color3B(239, 78, 100));
            setVisible(true);
            break;
        case 16384:
            background -> setColor(Color3B(234, 67, 70));
            setVisible(true);
            break;
        case 32768:
            background -> setColor(Color3B(118, 182, 220));
            setVisible(true);
            break;
        case 65536:
            background -> setColor(Color3B(97, 162, 225));
            setVisible(true);
            break;
        case 131072:
            background -> setColor(Color3B(19, 129, 193));
            setVisible(true);
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