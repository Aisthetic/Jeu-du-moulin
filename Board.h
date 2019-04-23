#pragma once
#include "cocos2d.h"
#include <array>
#include <vector>
USING_NS_CC;

class Board
{
    public:
        Board(cocos2d::Scene *layer, double rX, double rY);
        Board(int Data[6][7]);
        ~Board();

         void copyBoard(int tab[6][7]);

        void nextTurn(){

        }
		//public utility functions
		std::tuple<double, double> NumberToPos(int);//converts a token number(0-23) to the pos of the center of the containing rect
		int PosToNumber(double, double);
		int PosToGameSquareNumber(double, double);
		Rect getContainingRect(int, int);
    private:
		// Screen resolution parameters
		Size visibleSize;
		Point origin;
		float rX, rY;

		//Graphic datas
		Rect* squares;
		cocos2d::Scene* scene;
		Sprite* sprite;
		std::vector< std::tuple< int , Sprite* > > tokens;//A vector of couples (tokenNumber,Sprite)

		//Graphic functions
		void drawToken(Rect,int,bool);
		void moveToken(int token, int x, int y);

		//game related
		bool isGameSquare(int);
		std::array<int, 24> gameSquares = { 
			0,3,6,
			8,10,12,
			16,17,18,
		21,22,23,25,26,27,
			30,31,32,
			36,38,40,
			42,45,48 };

		//utility 
		////Credits: https://github.com/veracl/muehle/blob/master/Board.cpp

		///*!
		//*  Model of the game board
		//*  The game board is represented as a graph structure consisting of:
		//*  - Vertices (the intersecting points on the board, numbered from 0 to 23, going in rows from top to bottom and left to right)
		//*  - E	dges (the connections between the points)
		//*
		//*  For example, the array {0,1} in edges means that the points 0 and 1 are connected
		//*
		//*  An entry in the array vertices can be:
		//*  - 0: The point on the board is empty(
		//*  - 1: The point on the board is occupied by one of the human player's pieces
		//*  - 2: The point on the board is occupied by one of the AI player's pieces
		//*/

		std::array<int, 24> vertices = { { 0 } };	

		std::array<std::array<int, 2>, 32> edges = { {
			{ { 0,1 } },
		{ { 0,9 } },
		{ { 1,2 } },
		{ { 1,4 } },
		{ { 2,14 } },
		{ { 3,10 } },
		{ { 3,4 } },
		{ { 4,5 } },
		{ { 4,7 } },
		{ { 5,13 } },
		{ { 6,7 } },
		{ { 6,11 } },
		{ { 7,8 } },
		{ { 8,12 } },
		{ { 9,10 } },
		{ { 9,21 } },
		{ { 10,11 } },
		{ { 10,18 } },
		{ { 11,15 } },
		{ { 12,13 } },
		{ { 12,17 } },
		{ { 13,14 } },
		{ { 13,20 } },
		{ { 14,23 } },
		{ { 15,16 } },
		{ { 16,17 } },
		{ { 16,19 } },
		{ { 18,19 } },
		{ { 19,20 } },
		{ { 19,22 } },
		{ { 21,22 } },
		{ { 22,23 } }
			} };

		/*!
		*  Model of the mills
		*  possibleMillPositions stores all combinations of three points on the board that can form a mill
		*
		*  This array is used to detect mills during the game
		*
		*  For example, the array {0,1,2} in possibleMillPositions means that the points 0, 1 and 2 form a mill
		*/

		std::array<std::array<int, 3>, 16> possibleMillPositions = { {
		{ { 0,1,2 } },
		{ { 0,9,21 } },
		{ { 1,4,7 } },
		{ { 2,14,23 } },
		{ { 3,4,5 } },
		{ { 3,10,18 } },
		{ { 5,13,20 } },
		{ { 6,7,8 } },
		{ { 6,11,15 } },
		{ { 8,12,17 } },
		{ { 9,10,11 } },
		{ { 12,13,14 } },
		{ { 15,16,17 } },
		{ { 16,19,22 } },
		{ { 18,19,20 } },
		{ { 21,22,23 } }
			} };

		/*!
		*  The vector protectedPoints saves all points that are involved in mills and therefore cannot be removed
		*/

		std::vector<int> protectedPoints;



    friend class playerAI;
	friend class GameplayScene;

};


