//
//  TubeLayer.cpp
//  2048
//
//  Created by shino on 16/4/7.
//
//

#include "TubeLayer.hpp"

USING_NS_CC;

bool TubeLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    
    //自身布局，定义布局尺寸
    Size designSize = Director::getInstance() -> getOpenGLView() -> getDesignResolutionSize();
    Rect gameArea = Rect(40, 120, designSize.width - 80, designSize.width - 80);
    Rect nonGameArea = Rect(40, designSize.width + 120, designSize.width - 80, designSize.height - designSize.width - 200);
    
    //添加背景
    auto background = LayerColor::create(Color4B(0, 255, 224, 255));
    addChild(background);
    
    auto layerBackground = LayerColor::create(Color4B(240, 255, 224, 255));
    layerBackground -> setContentSize(gameArea.size);
    layerBackground -> setPosition(gameArea.origin);
    addChild(layerBackground);
    
    labelInit(nonGameArea);
    
    //生成4*4方块
    tubeInit(gameArea);
    
    /*绑定触摸事件*/
    touchInit(gameArea);
    return true;
}

void TubeLayer::labelInit(Rect labelArea) {
    auto tubeSize = Size((labelArea.size.width - 80) / 3, (labelArea.size.width - 80) / 3);
    highestTube = NumberTube::create(tubeSize, Vec2(tubeSize.width / 2 + labelArea.origin.x, labelArea.getMaxY() - tubeSize.height / 2));
    highestTubeNum = 2;
    highestTube -> setNum(highestTubeNum);
    addChild(highestTube);
    
    auto highestScoreBackground = LayerColor::create(Color4B(0, 224, 224, 255));
    highestScoreBackground -> setContentSize(Size(tubeSize.width, (tubeSize.height - 30) * 2 / 3));
    highestScoreBackground -> setPosition(Vec2(tubeSize.width + labelArea.origin.x + 40, labelArea.getMaxY() - (tubeSize.height - 30) * 2 / 3));
    addChild(highestScoreBackground);
    highestScoreLabel = Label::createWithSystemFont("最高得分\n0", "Arial", 40);
    highestScoreLabel -> setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    highestScoreLabel -> setPosition(Vec2(highestScoreBackground -> getContentSize().width / 2, highestScoreBackground -> getContentSize().height / 2));
    highestScoreBackground -> addChild(highestScoreLabel);
    
    auto scoreBackground = LayerColor::create(Color4B(0, 224, 224, 255));
    scoreBackground -> setContentSize(Size(tubeSize.width, (tubeSize.height - 30) * 2 / 3));
    scoreBackground -> setPosition(Vec2(tubeSize.width * 2 + 80 + labelArea.origin.x, labelArea.getMaxY() - (tubeSize.height - 30) * 2 / 3));
    addChild(scoreBackground);
    score = 0;
    auto tmpStr = Value(score).asString();
    scoreLabel = Label::createWithSystemFont("当前得分\n" + tmpStr, "Arial", 40);
    scoreLabel -> setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    scoreLabel -> setPosition(highestScoreLabel -> getPosition());
    scoreBackground -> addChild(scoreLabel);
    
    auto message = Label::createWithSystemFont("您的目标是 4096 分", "Arial", 60);
    message -> setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    message -> setPosition(Vec2(labelArea.getMidX(), labelArea.getMinY() + 40));
    addChild(message);
}

void TubeLayer::tubeInit(Rect tubeArea) {
    Size tubeSize = Size((tubeArea.size.width - 100) / 4, (tubeArea.size.height - 100) / 4);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            Vec2 tubePosition = Vec2(tubeSize.width / 2, tubeSize.width / 2);
            Vec2 tubeCoordinate = Vec2(x * 2, y * 2);
            auto scalar = tubeCoordinate + Vec2(1, 1);
            tubePosition.scale(scalar);
            tubePosition = tubePosition + tubeArea.origin + Vec2(20 * (x + 1), 20 * (y + 1));
            tube[y][x] = NumberTube::create(tubeSize, tubePosition);
            tube[y][x] -> setUpdateDelegator(this);
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
        if (moveDistance.length() > 100 && isTouchMoved == false) {
            int moveDirection;
            if (std::abs(moveDistance.x) > std::abs(moveDistance.y)) {
                if (moveDistance.x > 0) {
                    moveDirection = 2;
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
        tubeToRandom.getRandomObject() -> setRandomNum();
    }
    isMoveFinished = true;
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