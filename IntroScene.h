#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "CCVideoManager.h"
#include "CCMediaPlayer.h"
class IntroScene : public cocos2d::Layer
{
public:

    static cocos2d::Scene* createScene();
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(IntroScene);

private:
    void SwitchToMainMenu( float dt );

//private:
// void SwitchToMainMenu( float dt );

};