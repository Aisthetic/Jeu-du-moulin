#include "GameplayScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include "Enums.h"
#include "Board.h"
#include <algorithm>
USING_NS_CC;

#pragma region Visual functions
Scene* GameplayScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameplayScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


// on "init" you need to initialize your instance
bool GameplayScene::init()
{
	// init origin & visible size
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//The gameplay background adapted to screen resolution
	Sprite* background = Sprite::create("whiteBg.png");
	background->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background, -1);
	rX = visibleSize.width / background->getContentSize().width;
	rY = visibleSize.height / background->getContentSize().height;
	background->setScaleX(rX);
	background->setScaleY(rY);
	// init board, player and the CPU
	board = new Board(this, rX, rY);

	// Touch Events Handling
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);


	listener->onTouchBegan = CC_CALLBACK_2(GameplayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameplayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameplayScene::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GameplayScene::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	//scheduleOnce(schedule_selector(GameplayScene::spPlaysFirst), 5);
	//we're skiping starting animations for debug
	gameStarted = true;

	//Initializing game variables
	placingPhase = true;
	placedTokens = 0;
	selectedToken = -1;
	turn = false;
	currentAction = GameActions::placing;

	AI = new ComputerPlayer(board);
	return true;

	
}
#pragma endregion

#pragma region Event handlers
void GameplayScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
	Point touchPoint = touch->getLocation();
	for (int i = 0; i < 49; i++) {
		auto rect = board->squares[i];
		if (rect.containsPoint(touchPoint))
			std::cout << "touché " << i << "(" << touchPoint.x << "," << touchPoint.y << ")" << std::endl;
	}
	//if the click is on the board
	if (board->sprite->getBoundingBox().containsPoint(touchPoint)) {
		if (gameStarted) handleInGameClick(touchPoint.x,touchPoint.y);
	}

}
bool GameplayScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
	return true;
}

void GameplayScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {     }
void GameplayScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {     }
#pragma endregion

#pragma region Getters and setters
void GameplayScene::setGameType(GameTypes type) {
	std::cout << "Game type set."<< endl;
	gameType = type;
}
void GameplayScene::placingPhaseOver(std::array<int, 24>)
{
}
#pragma endregion

#pragma region Graphic messages

void GameplayScene::spPlaysFirst(float dt){
	auto cb = CallFunc::create([&]() {
		scheduleOnce(schedule_selector(GameplayScene::placingPhaseStart), 2);
	});
	updateTurn();
	showMessage("spongebobPlaysFirst.png", MessagesAnimations::fadeInAndOut,cb);
}

void GameplayScene::placingPhaseStart(float dt) {
	auto cb = CallFunc::create([&]() {
		//Starting the game
		gameStarted = true;
	});
	showMessage("placingPhaseStart.png", MessagesAnimations::fadeInAndOut,cb);
}

void GameplayScene::showReplayMessage(float dt)
{
	showMessage("placingPhaseStart.png", MessagesAnimations::fadeInAndOut, nullptr);
}

void GameplayScene::updateTurn()
{
	//display smth
	//creating the token
	std::string name = !turn ? "planktonToken.png" : "spongebobToken.png";
	Sprite* dot = Sprite::create(name.c_str());

	//adding the token to the scene
	dot->setPosition(Point(200,200));
	dot->setScale(rX / 10, rY / 10);
	addChild(dot, 10001);
}

void GameplayScene::showMessage(std::string message, MessagesAnimations animationType, CallFunc* callback) {
	//FadeInAndOut vars
	auto fadeIn = FadeIn::create(2.0f);
	auto fadeOut = FadeOut::create(2.0f);
	auto sp = Sprite::create(message.c_str());
	auto cb = CallFunc::create([]() {});//useless callback function needed between two animations to make the lib work since nullptr makes it bug
	auto fadeInAndOutSec = Sequence::create(fadeIn, cb, fadeOut, callback, nullptr);
	 
	switch (animationType)
	{
	case MessagesAnimations::fadeInAndOut:
		sp->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(sp, 101);
		sp->setOpacity(0);
		sp->setCascadeOpacityEnabled(true);
		rX = visibleSize.width / sp->getContentSize().width;
		rY = visibleSize.height / sp->getContentSize().height;
		sp->setScaleX(rX);
		sp->setScaleY(rY);
		sp->runAction(fadeInAndOutSec);
		break;
	default:
		break;
	}
	
}
void GameplayScene::askEnemyToPlay(float dt)
{
	//showMessage("askEnemyToPlay.png", MessagesAnimations::fadeInAndOut, nullptr);
	log("ASKING_ENEMY_TO_PLAY");
}

void GameplayScene::askPlayerToCapture(float dt)
{
	//showMessage("askPlayerToCapture.png", MessagesAnimations::fadeInAndOut, nullptr);
	log("ASKING_PLAYER_TO_CAPTURE");
}

void GameplayScene::anoucePlacingOver(float dt)
{
	//showMessage("anoucePlacingOver.png", MessagesAnimations::fadeInAndOut, nullptr);
	log("PLACING_PHASE_OVER");
}

void GameplayScene::askPlayerToPlaceToken(float dt)
{
	//showMessage("askPlayerToPlaceToken.png", MessagesAnimations::fadeInAndOut, nullptr);
	log("ASKING_PLAYER_TO_PLACE_TOKEN");
}

void GameplayScene::refuseClick(std::string reason)
{
	log(reason.c_str());
}
void GameplayScene::capture(int x, int y)
{
	int token = board->PosToGameSquareNumber(x, y);
	//We start by the graphics since it still needs the data before getting deleted
	for (int i = 0; i < board->tokens.size(); i++) {
		if (token == get<0>(board->tokens[i]))
		{
			removeChild(get<1>(board->tokens[i]), true);
			log("REMOVED_CHILD_" + token);
			board->tokens.erase(board->tokens.begin()+i);
		}
	}

	//Cleaning up the board data
	board->vertices[token] = 0;

	//reseting the capture variables
	selectedToken = -1;
	hasMill = false;
	if(placingPhase) currentAction = GameActions::placing;

	//checking if any mill has been borken after the capture
	checkIfMillIsBroken();
}
#pragma endregion

#pragma region Gameplay
void GameplayScene::handleInGameClick(int x,int y) {
	//temp vars
	auto touchedToken = board->PosToNumber(x, y);
	auto tempTurn = turn;

	//Initial checkings for possible actions accordingly to the game's rules
	//is the touched square a gameSquare ? 
	if (!board->isGameSquare(touchedToken))
		return refuseClick("TOUCHED_A_NONGAME_SQUARE");
	//can the player select any cell ? 
	if (!isFree(x, y) && !canSelect())
		return refuseClick("CANT_SELECT_ANY");

	//what cells a player cannot select
	if (currentAction == GameActions::selecting) {
		//case 1:the player is trying to capture a mill forming cell or the player cant capture at all (no ally mill formed)
		if (hasMill && !isAlly(x, y) && isUntouchable(x, y))
			return refuseClick("TRYING_TO_CAPTURE_UNTOUCHABLE_ENEMY");
		//case 2:the player is selecting a free cell
		if (isFree(x, y))
			return refuseClick("SELECTING_FREE_CELL");
		//case 3:the player has a mill and trying to capture an ally
		if (hasMill && isAlly(x, y))
			return refuseClick("TRYING_TO_CAPTURE_ALLY");
	}
	else if (currentAction == GameActions::placing) {
		//case 1:the chosen position is not free
		if (!isFree(x, y))
			return refuseClick("PLACING_OVER_CELL");
	}

	//Processing the position
	if (currentAction == GameActions::placing) {
		if (selectedToken != -1) moveToken(selectedToken, x, y);
		else placeToken(x, y);
		checkForMill();
		if(!hasMill) turn = !turn;//switch turn
	}
	else if(currentAction == GameActions::selecting){	
		if (hasMill && !isAlly(x, y)) {
			capture(x, y);
			turn = !turn;
		}
		else if (isAlly(x, y)) {
			currentAction = GameActions::placing;
			selectedToken = board->PosToGameSquareNumber(x,y);
			log("PLAYER_SELECTED_TOKEN_" + selectedToken);
		}
		else {
			log("UNKNOWN_GAMEACTION::SELECTING_CASE");
		}
	}

	//finalizing
	//case 0: the player must capture
	if (hasMill) {
		scheduleOnce(schedule_selector(GameplayScene::askPlayerToCapture), 0.5F);
	}
	//case 1: the player must replay since he failed his move or place the token he selected
	else if (tempTurn == turn) {
		if(selectedToken != -1) scheduleOnce(schedule_selector(GameplayScene::askPlayerToPlaceToken), 0.5F);
		else scheduleOnce(schedule_selector(GameplayScene::showReplayMessage), 0.5F);
	}
	//case 2: pvp , we ask the other player to player
	else if (gameType == GameTypes::pvp) {
		scheduleOnce(schedule_selector(GameplayScene::askEnemyToPlay), 0.5F);
	}
	//case 3: miniMax is the ennemy
	else if (gameType == GameTypes::pveMiniMax) {
		askMiniMaxToPlay();//not a graphic func
	}
	else if (gameType == GameTypes::pveAlphaBeta) {
		askAlpaBetaToPlay();//not graphic func
	}
	updateTurn();
	checkIfGameOver();
}
void GameplayScene::placeToken(int x, int y)
{
	auto token = board->PosToGameSquareNumber(x, y);
	board->drawToken(board->squares[board->PosToNumber(x,y)],token, turn);
	board->vertices[token] = turn ? 2 : 1;
	if (placingPhase) {
		placedTokens++;
		if (placedTokens > 17) {
			log("PLACING_PHASE_OVER");
			placingPhase = !placingPhase;
			currentAction = GameActions::selecting;
		}
	}
}

void GameplayScene::moveToken(int token, int x, int y)
{
	//managing visuals
	board->moveToken(token, x, y);

	//updating the data
	auto newToken = board->PosToGameSquareNumber(x, y);
	board->vertices[token] = 0;
	board->vertices[newToken] = turn ? 2 : 1;
	selectedToken = -1;
	currentAction = GameActions::selecting;
}

void GameplayScene::checkForMill() {
	//we're gonna use the data from board about possible mills
	//the data is a 16 sized array of 3 sized arrays of ints , each int represents a token
	//we're going to check if each of these 3 tokens are allies (between then and the current player)
	auto tokenSideOnVertice = turn ? 2 : 1;
	for (int i = 0; i < 16; i++) {
		//checking if there's an unconsumed mill on the board
		if (!millIsConsumed(board->possibleMillPositions[i]) && board->vertices[board->possibleMillPositions[i][0]]== tokenSideOnVertice && board->vertices[board->possibleMillPositions[i][1]] == tokenSideOnVertice && board->vertices[board->possibleMillPositions[i][2]] == tokenSideOnVertice) {
			log("MILL_DETECTED");
			currentAction = GameActions::selecting;
			hasMill = true;
			consumedMills.push_back(board->possibleMillPositions[i]);
			return;
		}
	} 
}

bool GameplayScene::checkForMill(std::array<int,24> vertices,bool player) {
	//we're gonna use the data from board about possible mills
	//the data is a 16 sized array of 3 sized arrays of ints , each int represents a token
	//we're going to check if each of these 3 tokens are allies (between then and the current player)
	auto tokenSideOnVertice = player ? 2 : 1;
	for (int i = 0; i < 16; i++) {
		//checking if there's an unconsumed mill on the board
		if (!millIsConsumed(board->possibleMillPositions[i]) &&	vertices[board->possibleMillPositions[i][0]] == tokenSideOnVertice && vertices[board->possibleMillPositions[i][1]] == tokenSideOnVertice && board->vertices[board->possibleMillPositions[i][2]] == tokenSideOnVertice) {
			log("CUSTOM_MILL_DETECTED");
			return true;
		}
	}
	return false;
}

void GameplayScene::checkIfMillIsBroken()
{
}


void GameplayScene::askMiniMaxToPlay()
{
	if (placingPhase) {
		log("ASKING_MINIMAX_TO_PLAY_PLACING_PHASE");
		int token = AI->putToken();
		std::cout << "MINIMAX_RETURNED_" << std::to_string(token) << std::endl;
		auto pos = board->NumberToPos(token);
		placeToken(std::get<0>(pos), std::get<1>(pos));
	}
	else {
		log("ASKING_MINIMAX_TO_PLAY_SECOND_PHASE");
		int finalToken = AI->putToken();
		auto pos = board->NumberToPos(finalToken);
		auto choice = AI->removeToken(board->vertices);
		int initialToken = std::get<1>(choice);
		moveToken(initialToken, std::get<0>(pos), std::get<1>(pos));
	}

	//check if cpature is needed
	checkForMill();
	if (hasMill) {
		auto choice = AI->capture(board->vertices);
		int token = std::get<1>(choice);
		auto pos = board->NumberToPos(token);
		std::cout << "MINIMAX_CAPTURING_" << std::to_string(token) << std::endl;
		capture(std::get<0>(pos), std::get<1>(pos));
	}
	turn = !turn;
}

void GameplayScene::askAlpaBetaToPlay()
{
}
void GameplayScene::checkIfGameOver()
{
	if (!placingPhase) {
		int tokensLeft = 0;
		for(int i = 0;i<24;i++){
			if (board->vertices[i] != 0) 
				tokensLeft = tokensLeft+1;
		}
		if (tokensLeft < 6) {
			//stop the game
			gameStarted = false;
			log("GAME_OVER");
		}
	}
}


#pragma endregion

#pragma region Utily
bool GameplayScene::isAlly(int x, int y) 
{
	int token = board->PosToGameSquareNumber(x, y);
	if (turn == false && board->vertices[token] == 1) return true;//spongebob's current player
	else if (turn == true && board->vertices[token] == 2) return true;//plankton's current player
	return false;
}


bool GameplayScene::isFree(double x, double y)
{
	int token = board->PosToGameSquareNumber(x, y);
	return board->vertices[token] == 0;
}

//checks if a cell cant be captured
bool GameplayScene::isUntouchable(int mill, int token)
{
	for (int i = 0; i < 16; i++) {
		auto mill = board->possibleMillPositions[i];
		if (isInMill(mill, token) && millCompleted(mill)) return true;
	}
	return false;
}

//checks if the player can select any cell
bool GameplayScene::canSelect()
{
	//case 1:still in placing phase
	if (placingPhase && !hasMill) return false;
	//case 2: already has a token selected
	if (selectedToken != -1) return false;
		
	return true;
}
bool GameplayScene::millIsConsumed(std::array<int, 3> mill)
{
	return std::find(consumedMills.begin(), consumedMills.end(), mill) != consumedMills.end();
}
bool GameplayScene::isInMill(std::array<int, 3> mill, int token)
{
	return mill[0] == token || mill[1] == token || mill[2] == token;
}
bool GameplayScene::millCompleted(std::array<int, 3> mill)
{
	return board->vertices[mill[0]] != 0 && board->vertices[mill[0]] == board->vertices[mill[1]] && board->vertices[mill[0]] == board->vertices[mill[2]];
}
#pragma endregion

