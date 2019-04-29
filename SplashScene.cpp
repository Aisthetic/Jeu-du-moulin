#include "SplashScene.h"
#include "SimpleAudioEngine.h"
#include "IntroScene.h"
#include "extensions/cocos-ext.h"


#define SCENE_TRANSITION_TIME 0.1f
#define SPLASH_SCENE_SHOW_TIME 0.1f
#define LOADING_INTERVAL 1



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

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	//splash
    auto background = Sprite::create("splash.jpg");
    background->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(background, -1);
	float rX = visibleSize.width / background->getContentSize().width;
	float rY = visibleSize.height / background->getContentSize().height;
    background->setScaleX(rX);
	background->setScaleY(rY);	
	
	//title
	/*auto title = Sprite::create("splashTitle.png");
	title->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y+300));
	title->setScale(rX, rY);
	addChild(title);*/

	//generating a label config for the loading bar captions
	TTFConfig labelConfig;
	labelConfig.fontFilePath = "spongefont.ttf";
	labelConfig.fontSize = 72;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.outlineSize = 0;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;

	//creating a loading bar
	loadingBar = ui::LoadingBar::create("loadingBar.png");
	loadingBar->setDirection(ui::LoadingBar::Direction::LEFT);
	loadingBar->setScale(0.2);
	loadingBar->setPercent(100);//to have a correct pos
	loadingBar->setPosition(Point(visibleSize.width / 2 + origin.x , visibleSize.height / 2 + origin.y - 200));
	loadingBar->setPercent(0);

	//putting a caption
	caption = Label::createWithTTF(labelConfig, "Initialisation du programe");
	caption->setColor(cocos2d::Color3B(255, 255, 255));
	addChild(caption);
	caption->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	/*scheduleOnce( schedule_selector( SplashScene::increaseLoadingBarPercent ), LOADING_INTERVAL );
	
	this->addChild(loadingBar);*/
	
	//skipping splash for debug
	scheduleOnce(schedule_selector(SplashScene::switchToIntro), SPLASH_SCENE_SHOW_TIME);


    return true;
}

void SplashScene::increaseLoadingBarPercent(float dt)
{
	if (loadingBar->getPercent() == 0) {
		loadingBar->setPercent(25);
		scheduleOnce(schedule_selector(SplashScene::increaseLoadingBarPercent), LOADING_INTERVAL);
		caption->setString("Preparation de la grill");
	}
	else if (loadingBar->getPercent() == 25) {
		loadingBar->setPercent(50);
		scheduleOnce(schedule_selector(SplashScene::increaseLoadingBarPercent), LOADING_INTERVAL);
		caption->setString("MOUZAW AYNI l'AI ?");
	}
	else if (loadingBar->getPercent() == 50) {
		loadingBar->setPercent(75);
		scheduleOnce(schedule_selector(SplashScene::increaseLoadingBarPercent), LOADING_INTERVAL);
		caption->setString("BRACE YOURSELF");
	}
	else if (loadingBar->getPercent() == 75) {
		loadingBar->setPercent(100);
		scheduleOnce(schedule_selector(SplashScene::increaseLoadingBarPercent), LOADING_INTERVAL);
		caption->setString("CALLING SPONGEBOB HEHE");
	}
	else if (loadingBar->getPercent() == 100) {
		scheduleOnce( schedule_selector( SplashScene::switchToIntro ), SPLASH_SCENE_SHOW_TIME );
	}


}

void SplashScene::switchToIntro( float dt )
{
	Scene *scene = IntroScene::createScene( );
	TransitionFade *transition = TransitionFade::create( SCENE_TRANSITION_TIME, scene );
	Director::getInstance( )->replaceScene( transition );
}