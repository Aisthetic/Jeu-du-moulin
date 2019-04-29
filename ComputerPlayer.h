#pragma once
#include <vector>
#include <array>
#include <Board.h>
#include <Enums.h>
class ComputerPlayer
{
public:
	ComputerPlayer(Board*);
	~ComputerPlayer();
	int putToken();
	std::tuple<int, int> ComputerPlayer::removeToken(std::array<int, 24> vertices);
	std::tuple<int, int> capture(std::array<int, 24> vertices);
private:
	//generate all the children of a given state , the output is a vector of 
	//tuples {token,state} where token is the shot done to generate the state
	Board* board;

	//Algorithmic functions
	//placement
	std::tuple<int, int>minimaxPlacementPhase(std::array<int, 24> vertices, int depth, bool isMax);
	int evaluatePlacement(std::array<int, 24>);
	//captures
	

	//heuristic functions
	int morrisClosed(std::array<int, 24> vertices, int token);
	int numberOfMills(std::array<int, 24> vertices, bool computer);
	int numberOfBlockedEnnemyTokens(std::array<int, 24> vertices, int token);
	int numberOfTokens(std::array<int, 24> vertices, bool computer);
	int numberOfMillsBlocked(std::array<int, 24> vertices, int token);

	//Utility
	bool placementPhaseOver(std::array<int, 24>);
	bool hasBlockedMill(std::array<int,24> vertices,std::array<int, 3> mill, int token);
	bool millIsFormed(std::array<int, 24> vertices, std::array<int, 3> mill, int side);
	bool millIsFull(std::array<int, 24> vertices, std::array<int, 3> mill);
	bool tokenInMill(std::array<int, 3> mill, int token);
	bool isBlockingMill(std::array<int, 24> vertices,std::array<int, 3> mill, int token);
};


