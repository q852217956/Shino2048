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
    Size screenSize = Director::getInstance() -> getVisibleSize();
    Size layerSize = Size(screenSize.width, screenSize.width) - Size(40, 40);
    setContentSize(layerSize);
    setPosition(Vec2(20, 100));
    
    //添加背景
    auto background = LayerColor::create(Color4B(255, 255, 255, 255));
    background -> setContentSize(layerSize);
    background -> setPosition(Vec2(0, 0));
    addChild(background);
    
    //生成4*4方块
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            Size tubeSize = layerSize / 4 - Size(40, 40);
            Vec2 tubePosition = setTubePosition(Vec2(x, y));
            tube[y][x] = NumberTube::create(tubeSize, tubePosition);
            addChild(tube[y][x]);
        }
    }
    
    //游戏开始时生成两个方块
    randomTubeNum(0);
    randomTubeNum(0);
    
    /*绑定触摸事件*/
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener -> setSwallowTouches(true);
    touchListener -> onTouchBegan = [&](Touch *touch, Event *event){
        /*if (isTouchFinished == false) {
            return false;
        }*/
        touchPoint = touch -> getLocation();
        auto touchRange = Rect(getPosition(), getContentSize());
        if (touchRange.containsPoint(touchPoint)) {
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
                    //isTouchFinished = false;
                    //moveRight();
                    moveDirection = 2;
                    touchMoveStack.push_back(moveDirection);
                } else {
                    //isTouchFinished = false;
                    //moveLeft();
                    moveDirection = 1;
                    touchMoveStack.push_back(moveDirection);
                }
            } else {
                if (moveDistance.y > 0) {
                    //isTouchFinished = false;
                    //moveUp();
                    moveDirection = 3;
                    touchMoveStack.push_back(moveDirection);
                } else {
                    //isTouchFinished = false;
                    //moveDown();
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
        if (!touchMoveStack.empty() && isTouchFinished == true){
            isTouchFinished = false;
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
    return true;
}

Vec2 TubeLayer::setTubePosition(Vec2 tubeCoordinate) {
    auto tubePosition = Vec2(getContentSize().width / 8, getContentSize().height / 8);
    tubeCoordinate.scale(2);
    auto scalar = tubeCoordinate + Vec2(1, 1);
    tubePosition.scale(scalar);
    return tubePosition;
}

void TubeLayer::randomTubeNum(float delayTime) {
    if (isFinished()) {
        return ;
    }
    int x, y;
    do {
        x = CCRANDOM_0_1() * 4;
        y = CCRANDOM_0_1() * 4;
    } while (tube[y][x] -> getNum() != 0);
    tube[y][x] -> setRandomNum();
    isTouchFinished = true;
}

bool TubeLayer::moveLeft() {
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
                    auto seqA = tubeToMove -> actionAddBy(tubeWillArrive);
                    tubeToMove -> runAction(seqA);
                    //tubeToMove -> action(tubeWillArrive, 1);
                    
                    tubeToMove = tubeToMoveStack.back();
                    tubeToMoveStack.popBack();
                    
                    auto seqB = tubeToMove-> actionAddTo(tubeWillArrive);
                    tubeToMove -> runAction(seqB);
                    //tubeToMove -> action(tubeWillArrive, 2);
                    
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                } else {
                    auto seq = tubeToMove -> actionMoveTo(tubeWillArrive);
                    tubeToMove -> runAction(seq);
                    //tubeToMove -> action(tubeWillArrive, 3);
                    
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                }
            } else {
                auto seq = tubeToMove -> actionMoveTo(tubeWillArrive);
                tubeToMove -> runAction(seq);
                //tubeToMove -> action(tubeWillArrive, 3);
                tubeWillArriveStack.popBack();
                tubeWillArrive = tubeWillArriveStack.back();
            }
        }
    }
    if (isMove) {
        scheduleOnce(schedule_selector(TubeLayer::randomTubeNum), 0.3);
    } else {
        isTouchFinished = true;
    }
    return isMove;
}

bool TubeLayer::moveRight() {
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
                    auto seqA = tubeToMove -> actionAddBy(tubeWillArrive);
                    tubeToMove -> runAction(seqA);
                    //tubeToMove -> action(tubeWillArrive, 1);
                    
                    tubeToMove = tubeToMoveStack.back();
                    tubeToMoveStack.popBack();
                    
                    auto seqB = tubeToMove -> actionAddTo(tubeWillArrive);
                    tubeToMove -> runAction(seqB);
                    //tubeToMove -> action(tubeWillArrive, 2);
                    
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                } else {
                    auto seq = tubeToMove -> actionMoveTo(tubeWillArrive);
                    tubeToMove -> runAction(seq);
                    //tubeToMove -> action(tubeWillArrive, 3);
                    
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                }
            } else {
                auto seq = tubeToMove -> actionMoveTo(tubeWillArrive);
                tubeToMove -> runAction(seq);
                //tubeToMove -> action(tubeWillArrive, 3);
                tubeWillArriveStack.popBack();
                tubeWillArrive = tubeWillArriveStack.back();
            }
        }
    }
    if (isMove) {
        scheduleOnce(schedule_selector(TubeLayer::randomTubeNum), 0.3);
    } else {
        isTouchFinished = true;
    }
    return isMove;
}

bool TubeLayer::moveUp() {
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
                    auto seqA = tubeToMove -> actionAddBy(tubeWillArrive);
                    tubeToMove -> runAction(seqA);
                    //tubeToMove -> action(tubeWillArrive, 1);
                    
                    tubeToMove = tubeToMoveStack.back();
                    tubeToMoveStack.popBack();
                    
                    auto seqB = tubeToMove -> actionAddTo(tubeWillArrive);
                    tubeToMove -> runAction(seqB);
                    //tubeToMove -> action(tubeWillArrive, 2);
                    
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                } else {
                    auto seq = tubeToMove -> actionMoveTo(tubeWillArrive);
                    tubeToMove -> runAction(seq);
                    //tubeToMove -> action(tubeWillArrive, 3);
                    
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                }
            } else {
                auto seq = tubeToMove -> actionMoveTo(tubeWillArrive);
                tubeToMove -> runAction(seq);
                //tubeToMove -> action(tubeWillArrive, 3);
                tubeWillArriveStack.popBack();
                tubeWillArrive = tubeWillArriveStack.back();
            }
        }
    }
    if (isMove) {
        scheduleOnce(schedule_selector(TubeLayer::randomTubeNum), 0.3);
    } else {
        isTouchFinished = true;
    }
    return isMove;
}

bool TubeLayer::moveDown() {
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
                    auto seqA = tubeToMove -> actionAddBy(tubeWillArrive);
                    tubeToMove -> runAction(seqA);
                    //tubeToMove -> action(tubeWillArrive, 1);
                    
                    tubeToMove = tubeToMoveStack.back();
                    tubeToMoveStack.popBack();
                    
                    auto seqB = tubeToMove-> actionAddTo(tubeWillArrive);
                    tubeToMove -> runAction(seqB);
                    //tubeToMove -> action(tubeWillArrive, 2);
                    
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                } else {
                    auto seq = tubeToMove -> actionMoveTo(tubeWillArrive);
                    tubeToMove -> runAction(seq);
                    //tubeToMove -> action(tubeWillArrive, 3);
                    
                    tubeWillArriveStack.popBack();
                    tubeWillArrive = tubeWillArriveStack.back();
                }
            } else {
                auto seq = tubeToMove -> actionMoveTo(tubeWillArrive);
                tubeToMove -> runAction(seq);
                //tubeToMove -> action(tubeWillArrive, 3);
                tubeWillArriveStack.popBack();
                tubeWillArrive = tubeWillArriveStack.back();
            }
        }
    }
    if (isMove) {
        scheduleOnce(schedule_selector(TubeLayer::randomTubeNum), 0.3);
    } else {
        isTouchFinished = true;
    }
    return isMove;
}

bool TubeLayer::isFinished() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 3; x++) {
            if (tube[y][x] -> getNum() == 0 || tube[y][x] -> getNum() == 0) {
                return false;
            }
            if (tube[y][x] -> getNum() == tube[y][x + 1] -> getNum()) {
                return false;
            }
        }
    }
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 3; y++) {
            if (tube[y][x] -> getNum() == tube[y + 1][x] -> getNum()) {
                return false;
            }
        }
    }
    return true;
}