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
    //Size layerSize =  Director::getInstance() -> getVisibleSize();
    auto rootLayer = GameLayer::create();
    this -> addChild(rootLayer);
    auto tubeLayer = TubeLayer::create();
    this -> addChild(tubeLayer);
    return true;
}

bool GameLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    auto backColor = LayerColor::create(Color4B(180, 170, 160, 255));
    addChild(backColor);
    return true;
}
