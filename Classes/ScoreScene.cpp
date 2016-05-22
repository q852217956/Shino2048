//
//  ScoreScene.cpp
//  2048
//
//  Created by shino on 16/4/7.
//
//

#include "ScoreScene.hpp"

USING_NS_CC;

bool ScoreScene::init() {
    if (!Scene::init()) {
        return false;
    }
    auto layer = ScoreLayer::create();
    this -> addChild(layer);
    return true;
}

bool ScoreLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    auto director = Director::getInstance();
    auto gameScene = GameScene::create();
    director -> replaceScene(gameScene);
    return true;
}