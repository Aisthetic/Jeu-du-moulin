#include "IntroScene.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "CCVideoManager.h"
#include "CCMediaPlayer.h"
#define SCENE_TRANSITION_TIME 0.1f
#define Intro_SCENE_SHOW_TIME 0.1f



USING_NS_CC;

Scene* IntroScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = IntroScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}



// on "init" you need to initialize your instance
bool IntroScene::init()
{
    // 1. super init first
    if ( !Layer::init() ) { return false; }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	//Keyboard key press handling to skip intro
	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [this,visibleSize,origin, eventListener](EventKeyboard::KeyCode keyCode, Event* event) {

		//Stopping the video
		CCVideoManager::Instance()->Stop();
		CCVideoManager::Instance()->DestroyInstance();
		IntroScene::SwitchToMainMenu(0);
		eventListener->release();
		//scheduleOnce(schedule_selector(IntroScene::SwitchToMainMenu), 0);
	};
	this->_eventDispatcher->addEventListenerWithFixedPriority(eventListener, 10);
	CCVideoManager::Instance()->PlayVideo("intro.mp4");

    return true;
}

void IntroScene::SwitchToMainMenu( float dt )
{
	Scene *scene = MenuScene::createScene( );
	TransitionFade *transition = TransitionFade::create( SCENE_TRANSITION_TIME, scene );
	Director::getInstance( )->replaceScene( transition );
}