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
    this -> addChild(tubeLayer, 10, "rootLayer");
    return true;
}

bool GameScene::readData() {
    auto tubeLayer = (TubeLayer*)this -> getChildByName("rootLayer");
    return tubeLayer -> readData();
}

bool GameScene::writeData() {
    auto tubeLayer = (TubeLayer*)this -> getChildByName("rootLayer");
    return tubeLayer -> writeData();
}