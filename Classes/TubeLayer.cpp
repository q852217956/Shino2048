//
//  TubeLayer.cpp
//  2048
//
//  Created by shino on 16/4/7.
//
//

#include "TubeLayer.hpp"

USING_NS_CC;

//定义边距
#define TUUBE_MARGIN 20
#define GAME_AREA_HEIGHT 120



bool TubeLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    
    //自身布局，定义布局尺寸
    Size designSize = Director::getInstance() -> getOpenGLView() -> getDesignResolutionSize();
    Rect gameArea = Rect(TUUBE_MARGIN * 2,
                         GAME_AREA_HEIGHT,
                         designSize.width - TUUBE_MARGIN * 4,
                         designSize.width - TUUBE_MARGIN * 4);
    Rect nonGameArea = Rect(TUUBE_MARGIN * 2,
                            designSize.width + TUUBE_MARGIN + GAME_AREA_HEIGHT,
                            designSize.width - TUUBE_MARGIN * 4,
                            designSize.height - designSize.width - TUUBE_MARGIN * 4 - GAME_AREA_HEIGHT);
    
    //添加背景颜色
    auto background = LayerColor::create(Color4B(0, 255, 224, 255));
    addChild(background);
    
    //添加游戏区域背景
    auto layerBackground = LayerColor::create(Color4B(0, 197, 197, 255));
    layerBackground -> setContentSize(gameArea.size);
    layerBackground -> setPosition(gameArea.origin);
    addChild(layerBackground);
    
    //生成4*4方块及方块背景
    this -> tubeInit(gameArea);
    
    //添加分数标签、当前最大方块
    this -> labelInit(nonGameArea);

    //读取游戏记录文件
    this -> readData();
    
    /*绑定触摸事件*/
    this -> touchInit(gameArea);
    return true;
}

void TubeLayer::tubeInit(Rect tubeArea) {
    Size tubeSize = Size((tubeArea.size.width - 100) / 4, (tubeArea.size.height - 100) / 4);
    //在方块下添加背景
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            Vec2 tubePosition = Vec2(tubeSize / 2);
            tubePosition.scale(Vec2(x * 2 + 1, y * 2 + 1));
            tubePosition = tubePosition + tubeArea.origin + Vec2(20 * (x + 1), 20 * (y + 1));
            auto tubeBackground = LayerColor::create(Color4B(174, 251, 251, 255));
            tubeBackground -> setContentSize(tubeSize);
            tubeBackground -> ignoreAnchorPointForPosition(false);
            tubeBackground -> setAnchorPoint(Vec2(0.5, 0.5));
            tubeBackground -> setPosition(tubePosition);
            addChild(tubeBackground);
        }
    }
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            /* 等边距布局，以横向为例：
             第n个方块位置 = 方块边距 * n + 方块尺寸 / 2 * (2 * n - 1) + 游戏区域偏移 */
            Vec2 tubePosition = Vec2(tubeSize / 2);
            tubePosition.scale(Vec2(x * 2 + 1, y * 2 + 1));
            tubePosition = tubePosition + tubeArea.origin + Vec2(20 * (x + 1), 20 * (y + 1));
            tube[y][x] = NumberTube::create(tubeSize, tubePosition);
            tube[y][x] -> setUpdateDelegator(this);
            addChild(tube[y][x]);
        }
    }
    this -> actionNum = 0;
}

void TubeLayer::labelInit(Rect labelArea) {
    //当前最大方块
    auto labelSize = Size((labelArea.size.width - TUUBE_MARGIN * 4) / 3,
                          (labelArea.size.width - TUUBE_MARGIN * 4) / 3);
    highestTube = NumberTube::create(labelSize,
                                     Vec2(labelSize.width / 2 + labelArea.origin.x,
                                          labelArea.getMaxY() - labelSize.height / 2));
    highestTube -> setNum(0);
    addChild(highestTube);
    
    //最高分
    auto highestScoreBackground = LayerColor::create(Color4B(0, 179, 224, 255));
    highestScoreBackground -> setContentSize(Size(labelSize.width, (labelSize.height - TUUBE_MARGIN) * 2 / 3));
    highestScoreBackground -> setPosition(Vec2(labelSize.width + labelArea.origin.x + TUUBE_MARGIN * 2,
                                               labelArea.getMaxY() - (labelSize.height - TUUBE_MARGIN) * 2 / 3));
    addChild(highestScoreBackground);
    auto highestScoreStr = Value(0).asString();
    highestScoreLabel = Label::createWithSystemFont("最高得分\n" + highestScoreStr, "Arial", 40);
    highestScoreLabel -> setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    highestScoreLabel -> setPosition(Vec2(highestScoreBackground -> getContentSize().width / 2,
                                          highestScoreBackground -> getContentSize().height / 2));
    highestScoreBackground -> addChild(highestScoreLabel);
    
    //当前分数
    auto scoreBackground = LayerColor::create(Color4B(0, 179, 224, 255));
    scoreBackground -> setContentSize(Size(labelSize.width, (labelSize.height - TUUBE_MARGIN) * 2 / 3));
    scoreBackground -> setPosition(Vec2(labelSize.width * 2 + TUUBE_MARGIN * 2 * 2 + labelArea.origin.x,
                                        labelArea.getMaxY() - (labelSize.height - TUUBE_MARGIN) * 2 / 3));
    addChild(scoreBackground);
    auto scoreStr = Value(0).asString();
    scoreLabel = Label::createWithSystemFont("当前得分\n" + scoreStr, "Arial", 40);
    scoreLabel -> setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    scoreLabel -> setPosition(highestScoreLabel -> getPosition());
    scoreBackground -> addChild(scoreLabel);
    
    auto nextTubeNum = Value(highestTube -> getNum() * 2).asString();
    message = Label::createWithSystemFont("您的现在的目标是 " + nextTubeNum, "Arial", 60);
    message -> setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    message -> setPosition(Vec2(labelArea.getMidX(), labelArea.getMinY() + 40));
    addChild(message);
}


bool TubeLayer::readData() {
    auto userData = UserDefault::getInstance();
    int highestScore = userData -> getIntegerForKey("HighestScore", 0);
    this -> highestScoreLabel -> setString("最高得分\n" + Value(highestScore).asString());
    if (userData -> getBoolForKey("GameOver", true)) {
        userData -> setBoolForKey("GameOver", false);
        
        //游戏开始时生成两个方块
        randomTubeNum();
        randomTubeNum();
    } else {
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                int n = y * 4 + x + 1;
                auto tubeNumKey = "Tube" + Value(n).asString();
                int tubeNum = userData -> getIntegerForKey(tubeNumKey.c_str());
                tube[y][x] -> setNum(tubeNum);
            }
        }
        int score = userData -> getIntegerForKey("Score");
        this -> scoreLabel -> setString("当前得分\n" + Value(score).asString());
        int highestTubeNum = userData -> getIntegerForKey("HighestTubeNum");
        this -> highestTube -> setNum(highestTubeNum);
    }
    return true;
}

bool TubeLayer::writeData(bool isGameOver) {
    auto userData = UserDefault::getInstance();
    if (isGameOver) {
        userData -> setBoolForKey("GameOver", true);
        userData -> setIntegerForKey("Score", 0);
        for (int n = 1; n <= 16; n++) {
            auto tubeNumKey = "Tube" + Value(n).asString();
            userData -> setIntegerForKey(tubeNumKey.c_str(), 0);
        }
        int highestScore = userData -> getIntegerForKey("HighestScore", 0);
        if (this -> score > highestScore) {
            userData -> setIntegerForKey("HighestScore", this -> score);
        }
    } else {
        userData -> setIntegerForKey("Score", this -> score);
        userData -> setIntegerForKey("HighestTubeNum", highestTube -> getNum());
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                int n = y * 4 + x + 1;
                auto tubeNumKey = "Tube" + Value(n).asString();
                userData -> setIntegerForKey(tubeNumKey.c_str(), this -> tube[y][x] -> getNum());
            }
        }
    }
    userData -> flush();
    return true;
}

void TubeLayer::touchInit(Rect touchArea) {
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener -> onTouchBegan = [&, touchArea](Touch *touch, Event *event){
        //判断是否在游戏区域内触摸
        if (touchArea.containsPoint(touch -> getLocation())) {
            isTouchMoved = false;
            return true;
        } else {
            return false;
        }
    };
    touchListener -> onTouchMoved = [&](Touch *touch, Event *event){
        auto moveDistance = Vec2(touch -> getStartLocation(), touch -> getLocation());
        //滑动距离大于100像素即触发滑动判定
        if (moveDistance.length() > 100 && isTouchMoved == false) {
            if (std::abs(moveDistance.x) > std::abs(moveDistance.y)) {
                if (moveDistance.x > 0) {
                    //每次滑动操作进入到滑动队列，防止同时处理多次操作
                    touchMoveQueue.push_back(Direction::RIGHT);
                } else {
                    touchMoveQueue.push_back(Direction::LEFT);
                }
            } else {
                if (moveDistance.y > 0) {
                    touchMoveQueue.push_back(Direction::UP);
                } else {
                    touchMoveQueue.push_back(Direction::DOWN);
                }
            }
            isTouchMoved = true;
        }
    };
    auto eventDispatcher = Director::getInstance() -> getEventDispatcher();
    eventDispatcher -> addEventListenerWithSceneGraphPriority(touchListener, this);
    
    this -> scheduleUpdate();
}

void TubeLayer::updateScore(int num) {
    score += num;
    auto tmpStr = "当前得分\n" + Value(score).asString();
    scoreLabel -> setString(tmpStr);
    if (num > this -> highestTube -> getNum()) {
        this -> highestTube -> setNum(num);
        auto nextTubeNum = Value(num * 2).asString();
        message -> setString("您的现在的目标是 " + nextTubeNum);
    }
}

void TubeLayer::updateActionNum() {
    if (this -> actionNum != 0) {
        this -> actionNum--;
        if (this -> actionNum == 0) {
            this -> randomTubeNum();
        }
    }
}

void TubeLayer::updateIsFinished() {
    bool isFinished = true;
    for (int y = 0; y < 4; y ++) {
        for (int x = 0; x < 4; x++) {
            if (tube[y][x] -> getNum() == 0) {
                isFinished = false;
            } else {
                if (x != 3) {
                    if (tube[y][x] -> getNum() == tube[y][x + 1] -> getNum()) {
                        isFinished = false;
                    }
                }
            }
        }
    }
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (y != 3) {
                if (tube[y][x] -> getNum() == tube[y + 1][x] -> getNum()) {
                    isFinished = false;
                }
            }
        }
    }
    if (isFinished) {
        this -> writeData(true);
        this -> newScene -> resetGame();
    }

}

void TubeLayer::update(float dt) {
    if (this -> actionNum != 0) {
        return;
    }
    if (!touchMoveQueue.empty()) {
        auto moveDirection = touchMoveQueue.front();
        touchMoveQueue.pop_back();
        move(moveDirection);
    }
}

void TubeLayer::randomTubeNum() {
    Vector<NumberTube*> tubeToRandom;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (tube[y][x] -> getNum() == 0) {
                tubeToRandom.pushBack(tube[y][x]);
            }
        }
    }
    if(!tubeToRandom.empty()) {
        //只从数字为0的空方块中随机生成新的数字
        tubeToRandom.getRandomObject() -> setRandomNum();
    }
}

void TubeLayer::setNewGame(ResetGameDelegate *newGame) {
    this -> newScene = newGame;
}

std::array<std::array<NumberTube*, 4>, 4> TubeLayer::arrangeArray(Direction direction) {
    std::array<std::array<NumberTube*, 4>, 4> tmp;
    switch (direction) {
        case Direction::LEFT :
            for (int y = 0, y2 = 0; y < 4; y++, y2++) {
                for (int x = 3, x2 = 0; x >= 0; x--, x2++) {
                    tmp[y][x2] = tube[y][x];
                }
            }
            break;
        case Direction::RIGHT :
            for (int y = 0, y2 = 0; y < 4; y++, y2++) {
                for (int x = 0, x2 = 0; x < 4; x++, x2++) {
                    tmp[y][x] = tube[y][x];
                }
            }
            break;
        case Direction::UP :
            for (int x = 0, y2 = 0; x < 4; x++, y2++) {
                for (int y = 0, x2 = 0; y < 4; y++, x2++) {
                    tmp[y2][x2] = tube[y][x];
                }
            }
            break;
        case Direction::DOWN :
            for (int x = 0, y2 = 0; x < 4; x++, y2++) {
                for (int y = 3, x2 = 0; y >= 0; y--, x2++) {
                    tmp[y2][x2] = tube[y][x];
                }
            }
            break;
    }
    return tmp;
}

void TubeLayer::move(Direction direction) {
    bool isMove = false;
    int num = 0;
    Vector<NumberTube*> tubeToMoveStack, tubeWillArriveStack;
    auto tubeArray = this -> arrangeArray(direction);
    for (int y = 0; y < 4; y++) {
        tubeWillArriveStack.clear();
        for (int x = 0; x < 4; x++) {
            tubeWillArriveStack.pushBack(tubeArray[y][x]);
            if (tubeArray[y][x] -> getNum() != 0) {
                tubeToMoveStack.pushBack(tubeArray[y][x]);
            } else {
                if (!tubeToMoveStack.empty()) {
                    isMove = true;
                }
            }
        }
        auto tubeWillArrive = tubeWillArriveStack.back();
        while (!tubeToMoveStack.empty()) {
            auto tubeToMove = tubeToMoveStack.back();
            tubeToMoveStack.popBack();
            if (!tubeToMoveStack.empty()) {
                if (tubeToMove -> getNum() == tubeToMoveStack.back() -> getNum()) {
                    isMove = true;
                    tubeToMove -> runActionAddBy(tubeWillArrive);
                    num++;
                    tubeToMove = tubeToMoveStack.back();
                    tubeToMoveStack.popBack();
                    
                    tubeToMove-> runActionAddTo(tubeWillArrive);
                    num++;
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                } else {
                    tubeToMove -> runActionMoveTo(tubeWillArrive);
                    num++;
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                }
            } else {
                tubeToMove -> runActionMoveTo(tubeWillArrive);
                num++;
                tubeWillArriveStack.popBack();
                tubeWillArrive = tubeWillArriveStack.back();
            }
        }
    }
    if (isMove) {
        this -> actionNum = num;
    }
}