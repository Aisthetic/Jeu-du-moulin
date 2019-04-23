#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
using namespace std;
using namespace ui;

class MenuScene : public cocos2d::Layer
{
public:



    static cocos2d::Scene* createScene();
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(MenuScene);

	void ShowMenu(Layer* menutoShow, Layer* menutoHide);

private:
	// First Menu
	Layer* menu1;
	Layer* menu2;

	// Screen Size and it's origen after scaling
	Size visibleSize;
	Point origin;
	double rX, rY;
	void switchToMenu(float dt);


};