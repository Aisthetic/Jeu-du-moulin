#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>

using namespace std;

class Player
{
    public:
        Player();
        ~Player();
        int getPos();

	private:
		int R, C;

    friend class Board;
	friend class GameplayScene;
};

class PlayerAI: Player
{
    public:
        PlayerAI(){PLUS_INFINITY=512; MINUS_INFINITY=-512; nbreNoeuds=0;}
        int minimax(int state[6][7], int currentDepth, bool isMax, int i, int j);
        int alpha_beta(int state[6][7], int currentDepth, bool isMax, int i, int j, int alpha, int beta);

    private:
        int finalDepth;
        int PLUS_INFINITY;
        int MINUS_INFINITY;
        int nbreNoeuds;

    friend class Board;
	friend class GameplayScene;

};

#endif // PLAYER_H
