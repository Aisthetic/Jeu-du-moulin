#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "IntroScene.h"
#include "extensions/cocos-ext.h"
class SplashScene : public cocos2d::Layer
{
public:

    static cocos2d::Scene* createScene();
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(SplashScene);

private:
	cocos2d::ui::LoadingBar* loadingBar;
	cocos2d::Label *caption;
    void increaseLoadingBarPercent( float dt );
	void switchToIntro(float dt);
};