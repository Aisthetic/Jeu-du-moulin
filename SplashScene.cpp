#include "SplashScene.h"
#include "SimpleAudioEngine.h"
#include "IntroScene.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

#define SCENE_TRANSITION_TIME 0.1f
#define SPLASH_SCENE_SHOW_TIME 0.1f



USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	printf("Debugging Window:\n");
    // return the scene
    return scene;
}



// on "init" you need to initialize your instance
bool SplashScene::init()
{
    // 1. super init first
    if ( !Layer::init() ) { return false; }


    //Add Background + transition to next scene
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
	auto bubblesbg = Sprite::create("bubblesBg.jpg");
	bubblesbg->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bubblesbg, -1.5);

	float rX = visibleSize.width / bubblesbg->getContentSize().width;
	float rY = visibleSize.height / bubblesbg->getContentSize().height;
	bubblesbg->setScaleX(rX);
	bubblesbg->setScaleY(rY);

    auto background = Sprite::create("splash.png");
    background->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(background, -1);
	
	rX = visibleSize.width / background->getContentSize().width;
    rY = visibleSize.height / background->getContentSize().height;
    background->setScaleX(rX);
	background->setScaleY(rY);
    scheduleOnce( schedule_selector( SplashScene::SwitchToIntro ), SPLASH_SCENE_SHOW_TIME );


    return true;
}

void SplashScene::SwitchToIntro( float dt )
{
	Scene *scene = IntroScene::createScene( );
	TransitionFade *transition = TransitionFade::create( SCENE_TRANSITION_TIME, scene );
	Director::getInstance( )->replaceScene( transition );
}