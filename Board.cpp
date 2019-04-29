#include <iostream>
#include <cstdlib>
#include <string.h>
#include "Board.h"
#include <iostream>


using namespace std;




Board::Board(cocos2d::Scene *layer, double _rX, double _rY)
{
  
	//Resolution parameters
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();	
	scene = layer;

	this->rX = _rX;
	this->rY = _rY;
	//Board Background
	sprite = Sprite::create("board.png");
	sprite->setPosition(Point(visibleSize.width / 2 + origin.x - visibleSize.width / 33, visibleSize.height / 2 + origin.y));
	sprite->setScale(rX, rY);
	layer->addChild(sprite,100);

	//Columns areas
	Rect BoardRect = sprite->getBoundingBox();

	//dividing the board into boardDivisions
	double bd = BoardRect.size.width / 12;//Rect is a square here so a div on the heigh equals a div on the width

	//Dividing the board into squares, each of them can contain a gameBox
	//we're dealing with a 7x7 grid , first and last row are special
	squares = new Rect[49];
	
	//my convential origins
	float Xo = BoardRect.origin.x - bd;
	float Yo = BoardRect.getMaxY() + bd;

	//Dividing the boarding into squares
	//on boucle sur les lignes(0-7)
	for (int j = 0; j < 7; j++) {
		squares[j * 7].setRect(Xo, Yo - 2 * (j + 1)*bd, 2 * bd, 2 * bd);	

		//on boucles sur les colones liées à chaque ligne
		for (int i = (j * 7) + 1; i < (j * 7) + 7; i++) {
			squares[i].setRect(squares[i - 1].getMaxX(), squares[i - 1].getMinY(), 2 * bd, 2 * bd);
		}
	}


}

void Board::drawToken(Rect square,int squareNumber,bool side) {
	//creating the token
	std::string name = side == 0 ? "planktonToken.png" : "spongebobToken.png";
	Sprite* dot = Sprite::create(name.c_str());

	//adding the token to the scene
	dot->setPosition(Point(square.getMidX(),square.getMidY()));
	dot->setScale(rX / 10, rY / 10);
	scene->addChild(dot, 10001);

	//saving the sprite on the vectos
	tokens.push_back({squareNumber,dot});
}

void Board::moveToken(int token, int x, int y)
{
	//creating the action(animation)
	auto moveTo = MoveTo::create(1, Vec2(x,y));
	//applying it to the token's sprite
	for (int i = 0; i < tokens.size(); i++) {
		if (token == get<0>(tokens[i]))
		{
			get<1>(tokens[i])->runAction(moveTo);
			log("RUNING_MOVE_ACTION_ON_TOKEN_" + token);
		}
	}
}

bool Board::isGameSquare(int square) {
	for (int i = 0; i < 24; i++) {
		if (gameSquares[i] == square) {
			return true;
		}
	}
	return false;
}

Board::~Board()
{
    //dtor
}


std::tuple<double, double> Board::NumberToPos(int token)
{
	return std::make_tuple(squares[gameSquares[token]].getMidX(),squares[gameSquares[token]].getMidY());
}

//converts a pos to a game token number(0 to 23)
int Board::PosToGameSquareNumber(double x, double y)
{
	Point touchPoint(x,y);
	int token = PosToNumber(x, y);
	for (int i = 0; i < 24; i++) {
		if (gameSquares[i] == token) return i;
	}
	log("pos is out of board range of game squares");
	return -1;
}

//converts a pos to a token number(0 to 48)
int Board::PosToNumber(double x, double y)
{
	Point touchPoint(x,y);
	for (int i = 0; i < 49; i++) {
		auto rect = squares[i];
		if (rect.containsPoint(touchPoint))
			return i;
	}
	log("pos is out of board range of squares");
	return -1;
}

	






