#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "Board.h"
#include "Enums.h"
#include <vector>
#include "ComputerPlayer.h"

USING_NS_CC;
using namespace std;
using namespace ui;

class GameplayScene : public cocos2d::Scene
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	void setGameType(GameTypes);

	void placingPhaseOver(std::array<int,24>);	
	bool checkForMill(std::array<int, 24>,bool player);
	CREATE_FUNC(GameplayScene);

private:
	//Local fields
	bool gameStarted;
	GameTypes gameType;
	GameActions currentAction;
	bool turn;
	int selectedToken;
	bool placingPhase;
	int placedTokens;
	bool hasMill;
	std::vector<std::array<int, 3>> consumedMills;//represents the mills that have already been used to capture a token
	ComputerPlayer* AI;
	// Touch event listeners
	EventListenerTouchOneByOne * listener;
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);
	
	//Visual elements
	Sprite* token;
	Board* board;

	// Screen Size and it's origen after scaling
	Size visibleSize;
	Point origin;
	double rX, rY;

	//GamePlay funcs  
	void handleInGameClick(int,int);
	void placeToken(int, int);//handles logic/graphic part of the task 
	void moveToken(int token,int x,int y);
	bool isAlly(int, int);//checks if a token is an ally
	void capture(int, int);//captures a token from the board
	void askMiniMaxToPlay();
	void askAlpaBetaToPlay();
	void checkIfGameOver();
	//Mill related functions
	void checkForMill();
	void checkIfMillIsBroken();

	//Graphics funcs
	//Game info management
	void updateTurn();
	//GraphicMessages
	void showMessage(std::string message, MessagesAnimations animationType, CallFunc* callback);//shows a message with an animation
	void spPlaysFirst(float dt);//sp always plays first :)
	void placingPhaseStart(float dt);
	void showReplayMessage(float dt);
	void askEnemyToPlay(float dt);
	void askPlayerToCapture(float dt);
	void anoucePlacingOver(float dt);	
	void askPlayerToPlaceToken(float dt);

	//Utility funcs
	bool isFree(double, double);//checks if a cell is free
	void refuseClick(std::string reason);
	bool isUntouchable(int, int);//checks if a cell is part of a mill or if the current player cant capture(no ally mill)
	bool canSelect();//checks if a player can select any cell
	bool millIsConsumed(std::array<int,3> mill);
	bool isInMill(std::array<int, 3> mill, int token);//checks if a token is in a mill
	bool millCompleted(std::array<int, 3> mill);
	friend class MenuScene;

};