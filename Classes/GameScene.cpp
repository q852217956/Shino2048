//
//  GameScene.cpp
//  2048
//
//  Created by shino on 16/4/7.
//
//

#include "GameScene.hpp"
#include "TubeLayer.hpp"

USING_NS_CC;

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }
    auto rootLayer = GameLayer::create();
    this -> addChild(rootLayer, 0, "rootLayer");
    setGameScore(0);
    auto tubeLayer = TubeLayer::create();
    this -> addChild(tubeLayer);
    return true;
}

int GameScene::getGameScore() {
    return gameScore;
}

void GameScene::setGameScore(int score) {
    gameScore += score;
    GameLayer *tmpLayer = (GameLayer*)getChildByName("rootLayer");
    tmpLayer -> setScoreLabelNum(gameScore);
}

bool GameLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    auto backColor = LayerColor::create(Color4B(180, 170, 160, 255));
    addChild(backColor);
    auto labelBackColor = LayerColor::create(Color4B(255, 0, 255, 255), 200, 60);
    labelBackColor -> setPosition(Vec2(400, 1000));
    addChild(labelBackColor);
    scoreLabel = Label::createWithSystemFont("0", "Arial", 40);
    scoreLabel -> setAnchorPoint(Vec2(0, 0));
    scoreLabel -> setAlignment(TextHAlignment::CENTER);
    scoreLabel -> setPosition(Vec2(400, 1000));
    addChild(scoreLabel);
    return true;
}

void GameLayer::setScoreLabelNum(int num) {
    scoreLabel -> setString(Value(num).asString());
}