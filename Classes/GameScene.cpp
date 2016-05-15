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
    auto tubeLayer = TubeLayer::create();
    tubeLayer -> setNewGame(this);
    tubeLayer -> setName("rootLayer");
    this -> addChild(tubeLayer);
    return true;
}

bool GameScene::readData() {
    auto tubeLayer = (TubeLayer*)this -> getChildByName("rootLayer");
    return tubeLayer -> readData();
}

bool GameScene::writeData(bool isGameOver) {
    auto tubeLayer = (TubeLayer*)this -> getChildByName("rootLayer");
    return tubeLayer -> writeData(isGameOver);
}

void GameScene::resetGame() {
    auto director = Director::getInstance();
    auto newGameScene = GameScene::create();
    auto appDelegate = (AppDelegate*)Application::getInstance();
    appDelegate -> setDataDelegator(newGameScene);
    director -> replaceScene(newGameScene);
}