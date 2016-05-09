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
    Rect gameArea = Rect(TUUBE_MARGIN * 2, GAME_AREA_HEIGHT,
                         designSize.width - TUUBE_MARGIN * 4, designSize.width - TUUBE_MARGIN * 4);
    Rect nonGameArea = Rect(TUUBE_MARGIN * 2, designSize.width + TUUBE_MARGIN + GAME_AREA_HEIGHT,
                            designSize.width - TUUBE_MARGIN * 4, designSize.height - designSize.width - TUUBE_MARGIN * 4 - GAME_AREA_HEIGHT);
    
    //添加背景颜色
    auto background = LayerColor::create(Color4B(0, 255, 224, 255));
    addChild(background);
    
    //添加游戏区域背景
    auto layerBackground = LayerColor::create(Color4B(0, 197, 197, 255));
    layerBackground -> setContentSize(gameArea.size);
    layerBackground -> setPosition(gameArea.origin);
    addChild(layerBackground);
    
    //添加分数标签、当前最大方块
    labelInit(nonGameArea);
    
    //生成4*4方块及方块背景
    tubeInit(gameArea);
    
    /*绑定触摸事件*/
    touchInit(gameArea);
    return true;
}

void TubeLayer::labelInit(Rect labelArea) {
    //当前最大方块
    auto labelSize = Size((labelArea.size.width - TUUBE_MARGIN * 4) / 3, (labelArea.size.width - TUUBE_MARGIN * 4) / 3);
    highestTube = NumberTube::create(labelSize,
                                     Vec2(labelSize.width / 2 + labelArea.origin.x, labelArea.getMaxY() - labelSize.height / 2));
    highestTubeNum = 2;
    highestTube -> setNum(highestTubeNum);
    addChild(highestTube);
    
    //最高分
    auto highestScoreBackground = LayerColor::create(Color4B(0, 179, 224, 255));
    highestScoreBackground -> setContentSize(Size(labelSize.width, (labelSize.height - TUUBE_MARGIN) * 2 / 3));
    highestScoreBackground -> setPosition(Vec2(labelSize.width + labelArea.origin.x + TUUBE_MARGIN * 2,
                                               labelArea.getMaxY() - (labelSize.height - TUUBE_MARGIN) * 2 / 3));
    addChild(highestScoreBackground);
    highestScoreLabel = Label::createWithSystemFont("最高得分\n0", "Arial", 40);
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
    score = 0;
    auto scoreStr = Value(score).asString();
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

void TubeLayer::tubeInit(Rect tubeArea) {
    Size tubeSize = Size((tubeArea.size.width - 100) / 4, (tubeArea.size.height - 100) / 4);
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
            log("background position: %f, %f", tubePosition.x, tubePosition.y);
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
            log("tube position: %f, %f", tubePosition.x, tubePosition.y);
            addChild(tube[y][x]);
        }
    }
    
    //游戏开始时生成两个方块
    randomTubeNum(0);
    randomTubeNum(0);
}

void TubeLayer::touchInit(Rect touchArea) {
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener -> setSwallowTouches(true);
    touchListener -> onTouchBegan = [&, touchArea](Touch *touch, Event *event){
        touchPoint = touch -> getLocation();
        //判断是否在游戏区域内触摸
        if (touchArea.containsPoint(touchPoint)) {
            isTouchMoved = false;
            return true;
        } else {
            return false;
        }
    };
    touchListener -> onTouchMoved = [&](Touch *touch, Event *event){
        auto currentPoint = touch -> getLocation();
        auto moveDistance = Vec2(touchPoint, currentPoint);
        //滑动距离大于100像素即触发滑动判定
        if (moveDistance.length() > 100 && isTouchMoved == false) {
            int moveDirection;
            if (std::abs(moveDistance.x) > std::abs(moveDistance.y)) {
                if (moveDistance.x > 0) {
                    //1为向左，2为向右，3为向上，4为向下
                    moveDirection = 2;
                    //每次滑动操作进入到滑动队列，防止同时处理多次操作
                    touchMoveStack.push_back(moveDirection);
                } else {
                    moveDirection = 1;
                    touchMoveStack.push_back(moveDirection);
                }
            } else {
                if (moveDistance.y > 0) {
                    moveDirection = 3;
                    touchMoveStack.push_back(moveDirection);
                } else {
                    moveDirection = 4;
                    touchMoveStack.push_back(moveDirection);
                }
            }
            isTouchMoved = true;
        }
    };
    auto eventDispatcher = Director::getInstance() -> getEventDispatcher();
    eventDispatcher -> addEventListenerWithSceneGraphPriority(touchListener, this);
    
    //每桢检查滑动队列状态，如果滑动队列不为空，且上一次滑动动画结束，则进行下一次动画
    schedule([=](float dt){
        if (!touchMoveStack.empty() && isMoveFinished == true){
            isMoveFinished = false;
            auto moveDirection = touchMoveStack.front();
            touchMoveStack.pop_back();
            switch (moveDirection) {
                case 1:
                    this -> moveLeft();
                    break;
                case 2:
                    this -> moveRight();
                    break;
                case 3:
                    this -> moveUp();
                    break;
                case 4:
                    this -> moveDown();
                    break;
                default:
                    break;
            }
        }
    }, "touchMove");
}

void TubeLayer::updateScore(int num) {
    score += num;
    auto tmpStr = "当前得分\n" + Value(score).asString();
    scoreLabel -> setString(tmpStr);
    if (num > highestTubeNum) {
        highestTubeNum = num;
        highestTube -> setNum(highestTubeNum);
        auto nextTubeNum = Value(num * 2).asString();
        message -> setString("您的现在的目标是 " + nextTubeNum);
    }
}

void TubeLayer::randomTubeNum(float delayTime) {
    Vector<NumberTube*> tubeToRandom;
    bool isFinished = true;
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            if(tube[y][x] -> getNum() == 0) {
                tubeToRandom.pushBack(tube[y][x]);
                isFinished = false;
            } else {
                if(x != 3) {
                    if(tube[y][x] -> getNum() == tube[y][x + 1] -> getNum()) {
                        isFinished = false;
                    }
                }
            }
        }
    }
    for(int x = 0; x < 4; x++) {
        for(int y = 0; y < 4; y++) {
            if(y != 3) {
                if(tube[y][x] -> getNum() == tube[y + 1][x] -> getNum()) {
                    isFinished = false;
                }
            }
        }
    }
    if(!tubeToRandom.empty()) {
        //只从数字为0的空方块中随机生成新的数字
        tubeToRandom.getRandomObject() -> setRandomNum();
        scheduleOnce([&](float dt){
            isMoveFinished = true;
        }, 0.03, "randomDelay");
    }
}

void TubeLayer::moveLeft() {
    bool isMove = false;
    Vector<NumberTube*> tubeToMoveStack;
    Vector<NumberTube*> tubeWillArriveStack;
    for (int y = 0; y < 4; y++) {
        tubeWillArriveStack.clear();
        for (int x = 3; x >= 0; x--) {
            tubeWillArriveStack.pushBack(tube[y][x]);
            if (tube[y][x] -> getNum() != 0) {
                tubeToMoveStack.pushBack(tube[y][x]);
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
                    tubeToMove = tubeToMoveStack.back();
                    tubeToMoveStack.popBack();
                    
                    tubeToMove-> runActionAddTo(tubeWillArrive);
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                } else {
                    tubeToMove -> runActionMoveTo(tubeWillArrive);
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                }
            } else {
                tubeToMove -> runActionMoveTo(tubeWillArrive);
                tubeWillArriveStack.popBack();
                tubeWillArrive = tubeWillArriveStack.back();
            }
        }
    }
    if (isMove) {
        scheduleOnce(schedule_selector(TubeLayer::randomTubeNum), 0.25);
    } else {
        isMoveFinished = true;
    }
}

void TubeLayer::moveRight() {
    bool isMove = false;
    Vector<NumberTube*> tubeToMoveStack;
    Vector<NumberTube*> tubeWillArriveStack;
    for (int y = 0; y < 4; y++) {
        tubeWillArriveStack.clear();
        for (int x = 0; x < 4; x++) {
            tubeWillArriveStack.pushBack(tube[y][x]);
            if (tube[y][x] -> getNum() != 0) {
                tubeToMoveStack.pushBack(tube[y][x]);
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
                    tubeToMove = tubeToMoveStack.back();
                    tubeToMoveStack.popBack();
                    
                    tubeToMove -> runActionAddTo(tubeWillArrive);
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                } else {
                    tubeToMove -> runActionMoveTo(tubeWillArrive);
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                }
            } else {
                tubeToMove -> runActionMoveTo(tubeWillArrive);
                tubeWillArriveStack.popBack();
                tubeWillArrive = tubeWillArriveStack.back();
            }
        }
    }
    if (isMove) {
        scheduleOnce(schedule_selector(TubeLayer::randomTubeNum), 0.25);
    } else {
        isMoveFinished = true;
    }
}

void TubeLayer::moveUp() {
    bool isMove = false;
    Vector<NumberTube*> tubeToMoveStack;
    Vector<NumberTube*> tubeWillArriveStack;
    for (int x = 0; x < 4; x++) {
        tubeWillArriveStack.clear();
        for (int y = 0; y < 4; y++) {
            tubeWillArriveStack.pushBack(tube[y][x]);
            if (tube[y][x] -> getNum() != 0) {
                tubeToMoveStack.pushBack(tube[y][x]);
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
                    tubeToMove = tubeToMoveStack.back();
                    tubeToMoveStack.popBack();
                    
                    tubeToMove -> runActionAddTo(tubeWillArrive);
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                } else {
                    tubeToMove -> runActionMoveTo(tubeWillArrive);
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                }
            } else {
                tubeToMove -> runActionMoveTo(tubeWillArrive);
                tubeWillArriveStack.popBack();
                tubeWillArrive = tubeWillArriveStack.back();
            }
        }
    }
    if (isMove) {
        scheduleOnce(schedule_selector(TubeLayer::randomTubeNum), 0.25);
    } else {
        isMoveFinished = true;
    }
}

void TubeLayer::moveDown() {
    bool isMove = false;
    Vector<NumberTube*> tubeToMoveStack;
    Vector<NumberTube*> tubeWillArriveStack;
    for (int x = 0; x < 4; x++) {
        tubeWillArriveStack.clear();
        for (int y = 3; y >= 0; y--) {
            tubeWillArriveStack.pushBack(tube[y][x]);
            if (tube[y][x] -> getNum() != 0) {
                tubeToMoveStack.pushBack(tube[y][x]);
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
                    tubeToMove = tubeToMoveStack.back();
                    tubeToMoveStack.popBack();
                    
                    tubeToMove-> runActionAddTo(tubeWillArrive);
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                } else {
                    tubeToMove -> runActionMoveTo(tubeWillArrive);
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                }
            } else {
                tubeToMove -> runActionMoveTo(tubeWillArrive);
                tubeWillArriveStack.popBack();
                tubeWillArrive = tubeWillArriveStack.back();
            }
        }
    }
    if (isMove) {
        scheduleOnce(schedule_selector(TubeLayer::randomTubeNum), 0.25);
    } else {
        isMoveFinished = true;
    }
}